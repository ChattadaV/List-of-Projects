import xml.etree.ElementTree as ET
import csv
import math
import pandas as pd
import csv
from math import radians, cos, sin, sqrt, atan2
from datetime import datetime, timedelta
from astral.sun import sun
from astral import LocationInfo
import pytz
import folium


# Below is the code from kml_to_csv_final.py

def haversine_initial(lon1, lat1, lon2, lat2):
    # Convert decimal degrees to radians
    lon1, lat1, lon2, lat2 = map(math.radians, [lon1, lat1, lon2, lat2])
    # Haversine formula
    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = math.sin(dlat/2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon/2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    r = 6371  # Radius of Earth in kilometers
    return c * r * 1000  # Return distance in meters

def calculate_bearing(lon1, lat1, lon2, lat2):
    # Convert decimal degrees to radians
    lon1, lat1, lon2, lat2 = map(math.radians, [lon1, lat1, lon2, lat2])
    dlon = lon2 - lon1
    x = math.sin(dlon) * math.cos(lat2)
    y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(dlon)
    initial_bearing = math.atan2(x, y)
    # Convert bearing from radians to degrees
    initial_bearing = math.degrees(initial_bearing)
    compass_bearing = (initial_bearing + 360) % 360  # Normalize to 0-360 degrees
    return compass_bearing

def calculate_speed_altitude_course(input_csv, output_csv):
    with open(input_csv, 'r') as infile, open(output_csv, 'w', newline='') as outfile:
        reader = csv.DictReader(infile)
        fieldnames = ['Timestamp', 'Speed (knots)', 'Altitude (feet)', 'Course (degrees)', 'Latitude', 'Longitude', 'TailNumber', 'Source', 'GPSModelName', 'FlightTitle']
        writer = csv.DictWriter(outfile, fieldnames=fieldnames)
        writer.writeheader()
        
        previous_row = None
        for row in reader:
            if previous_row:
                lon1, lat1, alt1 = float(previous_row['Longitude']), float(previous_row['Latitude']), float(previous_row['Altitude'])
                lon2, lat2, alt2 = float(row['Longitude']), float(row['Latitude']), float(row['Altitude'])
                distance = haversine_initial(lon1, lat1, lon2, lat2)
                speed_mps = distance  # Speed in meters per second (since data is logged every second)
                speed_knots = speed_mps * 1.94384  # Convert speed to knots
                altitude_feet = alt2 * 3.28084  # Convert altitude to feet
                course = calculate_bearing(lon1, lat1, lon2, lat2)  # Calculate course
                writer.writerow({
                    'Timestamp': row['Timestamp'], 
                    'Speed (knots)': f"{speed_knots:.2f}", 
                    'Altitude (feet)': f"{altitude_feet:.2f}", 
                    'Course (degrees)': f"{course:.2f}", 
                    'Latitude': row['Latitude'],
                    'Longitude': row['Longitude'],
                    'TailNumber': row['TailNumber'],
                    'Source': row['Source'],
                    'GPSModelName': row['GPSModelName'],
                    'FlightTitle': row['FlightTitle']
                })
            previous_row = row

# Function to extract data from KML file and save to CSV
def kml_to_csv(input_kml_file, cleaned_csv_file):
    # Namespaces used in the KML file
    ns = {'kml': 'http://www.opengis.net/kml/2.2', 'gx': 'http://www.google.com/kml/ext/2.2'}

    # Parse the KML file
    tree = ET.parse(input_kml_file)
    root = tree.getroot()

    # Prepare headers for the CSV file
    headers = ['Timestamp', 'Longitude', 'Latitude', 'Altitude', 'Horizontal Acceleration', 'Vertical Acceleration', 'Course', 'Speed (Kts)', 'Altitude2', 'Bank', 'Pitch', 'Source', 'GPSModelName', 'FlightTitle', 'PilotName', 'TailNumber', 'PilotNotes', 'RouteWaypoints']

    # Open CSV file for writing
    with open(cleaned_csv_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(headers)

        # Extract data from gx:Track
        for placemark in root.findall('.//kml:Placemark', ns):
            track = placemark.find('gx:Track', ns)
            if track is not None:
                times = track.findall('kml:when', ns)
                coords = track.findall('gx:coord', ns)

                for when, coord in zip(times, coords):
                    timestamp = when.text
                    lon, lat, alt = coord.text.split()

                    # Extract SimpleArrayData
                    acc_horiz = root.find('.//gx:SimpleArrayData[@name="acc_horiz"]/gx:value', ns).text
                    acc_vert = root.find('.//gx:SimpleArrayData[@name="acc_vert"]/gx:value', ns).text
                    course = root.find('.//gx:SimpleArrayData[@name="course"]/gx:value', ns).text
                    speed_kts = root.find('.//gx:SimpleArrayData[@name="speed_kts"]/gx:value', ns).text
                    altitude = root.find('.//gx:SimpleArrayData[@name="altitude"]/gx:value', ns).text
                    bank = root.find('.//gx:SimpleArrayData[@name="bank"]/gx:value', ns).text
                    pitch = root.find('.//gx:SimpleArrayData[@name="pitch"]/gx:value', ns).text

                    # Extract ExtendedData values
                    source = root.find('.//kml:Data[@name="source"]/kml:value', ns).text
                    gps_model = root.find('.//kml:Data[@name="GPSModelName"]/kml:value', ns).text
                    flight_title = root.find('.//kml:Data[@name="flightTitle"]/kml:value', ns).text
                    pilot_name = root.find('.//kml:Data[@name="pilotName"]/kml:value', ns).text
                    tail_number = root.find('.//kml:Data[@name="tailNumber"]/kml:value', ns).text
                    pilot_notes = root.find('.//kml:Data[@name="pilotNotes"]/kml:value', ns).text
                    route_waypoints = root.find('.//kml:Data[@name="routeWaypoints"]/kml:value', ns).text

                    # Write data to CSV
                    writer.writerow([timestamp, lon, lat, alt, acc_horiz, acc_vert, course, speed_kts, altitude, bank, pitch, source, gps_model, flight_title, pilot_name, tail_number, pilot_notes, route_waypoints])



# Below is the code from csv_to_data_formatted.py

# Function to calculate the distance between two points using the Haversine formula
def haversine_final(lat1, lon1, lat2, lon2):
    R = 6371  # Radius of the Earth in kilometers
    dlat = radians(lat2 - lat1)
    dlon = radians(lon2 - lon1)
    a = sin(dlat / 2) ** 2 + cos(radians(lat1)) * cos(radians(lat2)) * sin(dlon / 2) ** 2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))
    distance = R * c
    return distance

# Function to determine if a timestamp is during night time (between sunset and sunrise without the ±1-hour buffer)
def is_night_flight(timestamp, latitude, longitude):
    location = LocationInfo(latitude=latitude, longitude=longitude)
    s = sun(location.observer, date=timestamp.date())
    sunset = s['sunset']
    sunrise = s['sunrise']
    
    # Convert timestamp to offset-aware datetime
    timestamp = timestamp.replace(tzinfo=pytz.UTC)

    return timestamp < sunrise or timestamp > sunset

# Function to determine if a landing is during the night with ±1-hour buffer
def is_night_landing(timestamp, latitude, longitude):
    location = LocationInfo(latitude=latitude, longitude=longitude)
    s = sun(location.observer, date=timestamp.date())
    sunset = s['sunset'] + timedelta(hours=1)
    sunrise = s['sunrise'] - timedelta(hours=1)
    
    # Convert timestamp to offset-aware datetime
    timestamp = timestamp.replace(tzinfo=pytz.UTC)

    return timestamp < sunrise or timestamp > sunset

# Function to calculate total flight time in hours with 1 decimal point
def calculate_flight_time(df):
    start_time = datetime.strptime(df['Timestamp'].iloc[0], '%Y-%m-%dT%H:%M:%S.%fZ')
    end_time = datetime.strptime(df['Timestamp'].iloc[-1], '%Y-%m-%dT%H:%M:%S.%fZ')
    total_duration = (end_time - start_time).total_seconds() / 3600  # Convert seconds to hours
    return round(total_duration, 1)

# Function to calculate cross-country time based on landings and unique airports visited
def calculate_cross_country_time(total_time, landings, unique_airports, departure_airport, arrival_airport):
    # If departure and arrival airports are the same
    if departure_airport == arrival_airport:
        if landings - unique_airports > -1:
            return total_time + (0.1 * ((unique_airports - landings) - 1)) + 0.1 # Account for extra landing to be correct
    # If departure and arrival airports are different
    else:
        if landings - unique_airports > -1:
            return total_time + (0.1 * ((unique_airports - landings) - 1))
    return total_time

def detect_steep_turns(df):
    steep_turns = 0
    turn_start_time = None
    turn_start_course = None
    degrees_turned = 0

    for i in range(1, len(df)):
        current_course = df['Course (degrees)'].iloc[i]
        previous_course = df['Course (degrees)'].iloc[i - 1]
        current_time = datetime.strptime(df['Timestamp'].iloc[i], '%Y-%m-%dT%H:%M:%S.%fZ')
        previous_time = datetime.strptime(df['Timestamp'].iloc[i - 1], '%Y-%m-%dT%H:%M:%S.%fZ')

        if turn_start_time is None:
            if abs(current_course - previous_course) > 5:
                turn_start_time = previous_time
                turn_start_course = previous_course
                degrees_turned = abs(current_course - previous_course)
        else:
            degrees_turned += abs(current_course - previous_course)
            if degrees_turned >= 345:
                turn_duration = (current_time - turn_start_time).total_seconds()
                if 15 <= turn_duration <= 30:
                    steep_turns += 1
                turn_start_time = None
                turn_start_course = None
                degrees_turned = 0

    return steep_turns

# Function to create a map with the flight path and log landings, steep turn, low passes, and flight time
def create_flight_path_map(
    cleaned_csv_file,
    make_and_model, 
    instrument_approach_num, 
    instrument_approach_type_location,
    airplane_single, 
    airplane_multi, 
    instrument_actual_time, 
    instrument_simulated_hood_time,
    instrument_simulator_ftd_time, 
    pic, 
    solo, 
    ground_training_received_time,
    flight_training_received_time, 
    flight_training_given_time
):

    output_csv = 'pilot_logbook.csv'

    # List of airports in Alabama, USA
    airport_coords = [
        {'ID': 'CRTA1', 'State': 'AL', 'Site': 'Cedar Point', 'Latitude': 30.308, 'Longitude': -88.14, 'Elevation': 20},
        {'ID': 'DPIA1', 'State': 'AL', 'Site': 'Dauphin Island', 'Latitude': 30.248, 'Longitude': -88.073, 'Elevation': 0},
        {'ID': 'FMOA1', 'State': 'AL', 'Site': 'Fort Morgan', 'Latitude': 30.228, 'Longitude': -88.025, 'Elevation': 0},
        {'ID': 'K0J4', 'State': 'AL', 'Site': 'Florala Muni', 'Latitude': 31.0447, 'Longitude': -86.3119, 'Elevation': 315},
        {'ID': 'K0J6', 'State': 'AL', 'Site': 'Headland Muni', 'Latitude': 31.3651, 'Longitude': -85.3112, 'Elevation': 358},
        {'ID': 'K11A', 'State': 'AL', 'Site': 'Clayton Muni', 'Latitude': 31.8815, 'Longitude': -85.4804, 'Elevation': 433},
        {'ID': 'K1A9', 'State': 'AL', 'Site': 'Prattville Arpt', 'Latitude': 32.4374, 'Longitude': -86.5098, 'Elevation': 213},
        {'ID': 'K1M4', 'State': 'AL', 'Site': 'Haleyville/Posey Fld', 'Latitude': 34.2832, 'Longitude': -87.5986, 'Elevation': 928},
        {'ID': 'K1R8', 'State': 'AL', 'Site': 'Bay Minette Muni', 'Latitude': 30.8691, 'Longitude': -87.8184, 'Elevation': 249},
        {'ID': 'K3A1', 'State': 'AL', 'Site': 'Cullman/Folsom Fld', 'Latitude': 34.268, 'Longitude': -86.858, 'Elevation': 965},
        {'ID': 'K4A6', 'State': 'AL', 'Site': 'Scottsboro Muni', 'Latitude': 34.688, 'Longitude': -86.006, 'Elevation': 627},
        {'ID': 'K4A9', 'State': 'AL', 'Site': 'Fort Payne/Isbell Fld', 'Latitude': 34.4759, 'Longitude': -85.717, 'Elevation': 896},
        {'ID': 'K79J', 'State': 'AL', 'Site': 'Andalusia/Benton Fld', 'Latitude': 31.3061, 'Longitude': -86.3902, 'Elevation': 305},
        {'ID': 'K8A0', 'State': 'AL', 'Site': 'Albertville Muni', 'Latitude': 34.2316, 'Longitude': -86.2481, 'Elevation': 1027},
        {'ID': 'K9A4', 'State': 'AL', 'Site': 'Courtland(AAF)', 'Latitude': 34.66, 'Longitude': -87.349, 'Elevation': 577},
        {'ID': 'KA08', 'State': 'AL', 'Site': 'Marion/Vaiden Fld', 'Latitude': 32.5167, 'Longitude': -87.3853, 'Elevation': 210},
        {'ID': 'KAIV', 'State': 'AL', 'Site': 'Aliceville/Downer Arpt', 'Latitude': 33.108, 'Longitude': -88.192, 'Elevation': 151},
        {'ID': 'KALX', 'State': 'AL', 'Site': 'Alexander City/Russell Fld', 'Latitude': 32.916, 'Longitude': -85.964, 'Elevation': 650},
        {'ID': 'KANB', 'State': 'AL', 'Site': 'Anniston Metro', 'Latitude': 33.5904, 'Longitude': -85.8479, 'Elevation': 614},
        {'ID': 'KASN', 'State': 'AL', 'Site': 'Talladega Muni', 'Latitude': 33.569, 'Longitude': -86.0519, 'Elevation': 522},
        {'ID': 'KATA1', 'State': 'AL', 'Site': 'Katrina Cut', 'Latitude': 30.258, 'Longitude': -88.213, 'Elevation': 13},
        {'ID': 'KAUO', 'State': 'AL', 'Site': 'Auburn Univ Arpt', 'Latitude': 32.617, 'Longitude': -85.4342, 'Elevation': 758},
        {'ID': 'KBFM', 'State': 'AL', 'Site': 'Mobile/Downtown Arpt', 'Latitude': 30.6147, 'Longitude': -88.063, 'Elevation': 23},
        {'ID': 'KBHM', 'State': 'AL', 'Site': 'Birmingham Intl', 'Latitude': 33.5655, 'Longitude': -86.7449, 'Elevation': 627},
        {'ID': 'KCKL', 'State': 'AL', 'Site': 'Centreville/Bib', 'Latitude': 32.9, 'Longitude': -87.25, 'Elevation': 459},
        {'ID': 'KCMD', 'State': 'AL', 'Site': 'Culman Rgnl', 'Latitude': 34.2722, 'Longitude': -86.8583, 'Elevation': 965},
        {'ID': 'KCQF', 'State': 'AL', 'Site': 'Fairhope/Callahan Arpt', 'Latitude': 30.4618, 'Longitude': -87.8749, 'Elevation': 69},
        {'ID': 'KDCU', 'State': 'AL', 'Site': 'Decatur/Pryor Fld', 'Latitude': 34.658, 'Longitude': -86.9434, 'Elevation': 591},
        {'ID': 'KDHN', 'State': 'AL', 'Site': 'Dothan Rgnl', 'Latitude': 31.3177, 'Longitude': -85.4432, 'Elevation': 371},
        {'ID': 'KDYA', 'State': 'AL', 'Site': 'Demopolis Muni', 'Latitude': 32.4641, 'Longitude': -87.9504, 'Elevation': 108},
        {'ID': 'KEDN', 'State': 'AL', 'Site': 'Enterprise Muni', 'Latitude': 31.299, 'Longitude': -85.9, 'Elevation': 338},
        {'ID': 'KEET', 'State': 'AL', 'Site': 'Alabaster/Shelby Cnty', 'Latitude': 33.1783, 'Longitude': -86.7818, 'Elevation': 564},
        {'ID': 'KEKY', 'State': 'AL', 'Site': 'Bessemer Arpt', 'Latitude': 33.314, 'Longitude': -86.925, 'Elevation': 699},
        {'ID': 'KEUF', 'State': 'AL', 'Site': 'Eufaula/Weedon Fld', 'Latitude': 31.9516, 'Longitude': -85.1312, 'Elevation': 285},
        {'ID': 'KGAD', 'State': 'AL', 'Site': 'Gadsden/NE Alabama Rgnl', 'Latitude': 33.9686, 'Longitude': -86.0917, 'Elevation': 554},
        {'ID': 'KGZH', 'State': 'AL', 'Site': 'Evergreen/Middleton Fld', 'Latitude': 31.4191, 'Longitude': -87.0484, 'Elevation': 253},
        {'ID': 'KHAB', 'State': 'AL', 'Site': 'Hamilton/Marion Cnty', 'Latitude': 34.117, 'Longitude': -87.998, 'Elevation': 413},
        {'ID': 'KHDL', 'State': 'AL', 'Site': 'Headland Muni', 'Latitude': 31.3641, 'Longitude': -85.3112, 'Elevation': 358},
        {'ID': 'KHEY', 'State': 'AL', 'Site': 'Hanchey(AHP)', 'Latitude': 31.348, 'Longitude': -85.655, 'Elevation': 312},
        {'ID': 'KHSV', 'State': 'AL', 'Site': 'Huntsville Intl', 'Latitude': 34.6441, 'Longitude': -86.7861, 'Elevation': 623},  
        {'ID': 'KHUA', 'State': 'AL', 'Site': 'Huntsville/Redstone AAF', 'Latitude': 34.676, 'Longitude': -86.6854, 'Elevation': 656},
        {'ID': 'KJFX', 'State': 'AL', 'Site': 'Jasper/Walker Cnty', 'Latitude': 33.9008, 'Longitude': -87.3092, 'Elevation': 472},
        {'ID': 'KJKA', 'State': 'AL', 'Site': 'Gulf Shores/Edwards Arpt', 'Latitude': 30.291, 'Longitude': -87.661, 'Elevation': 16},
        {'ID': 'KLOR', 'State': 'AL', 'Site': 'Ft Rucker/Lowe(AHP)', 'Latitude': 31.36, 'Longitude': -85.749, 'Elevation': 302},
        {'ID': 'KMDQ', 'State': 'AL', 'Site': 'Huntsville/Sharp Fld', 'Latitude': 34.866, 'Longitude': -86.559, 'Elevation': 725},
        {'ID': 'KMGM', 'State': 'AL', 'Site': 'Montgomery Rgnl', 'Latitude': 32.2997, 'Longitude': -86.4074, 'Elevation': 210},
        {'ID': 'KMOB', 'State': 'AL', 'Site': 'Mobile Rgnl', 'Latitude': 30.6882, 'Longitude': -88.2459, 'Elevation': 220},
        {'ID': 'KMSL', 'State': 'AL', 'Site': 'Muscle Shoals/NW Alabama Rgnl', 'Latitude': 34.7439, 'Longitude': -87.5997, 'Elevation': 558},
        {'ID': 'KMVC', 'State': 'AL', 'Site': 'Monroeville/Monroe Cnty', 'Latitude': 31.458, 'Longitude': -87.351, 'Elevation': 420},
        {'ID': 'KMXF', 'State': 'AL', 'Site': 'Maxwell AFB', 'Latitude': 32.3877, 'Longitude': -86.3724, 'Elevation': 154},
        {'ID': 'KNBJ', 'State': 'AL', 'Site': 'Barin Fld(NAS)', 'Latitude': 30.391, 'Longitude': -87.633, 'Elevation': 49},
        {'ID': 'KOZR', 'State': 'AL', 'Site': 'Ozark/Cairns AAF', 'Latitude': 31.2767, 'Longitude': -85.7105, 'Elevation': 295},
        {'ID': 'KPLR', 'State': 'AL', 'Site': 'Pell City/St Clair Cnty', 'Latitude': 33.5608, 'Longitude': -86.2463, 'Elevation': 476},
        {'ID': 'KPRN', 'State': 'AL', 'Site': 'Greenville/Crenshaw Mem', 'Latitude': 31.8467, 'Longitude': -86.6141, 'Elevation': 449},
        {'ID': 'KSCD', 'State': 'AL', 'Site': 'Sylacauga Muni', 'Latitude': 33.1732, 'Longitude': -86.2933, 'Elevation': 538},
        {'ID': 'KSEM', 'State': 'AL', 'Site': 'Selma/Craig Fld', 'Latitude': 32.3367, 'Longitude': -86.9836, 'Elevation': 157},
        {'ID': 'KSXS', 'State': 'AL', 'Site': 'Schell AFP', 'Latitude': 31.364, 'Longitude': -85.846, 'Elevation': 394},
        {'ID': 'KTCL', 'State': 'AL', 'Site': 'Tuscaloosa Rgnl', 'Latitude': 33.2122, 'Longitude': -87.6155, 'Elevation': 157},
        {'ID': 'KTOI', 'State': 'AL', 'Site': 'Troy Muni', 'Latitude': 31.8574, 'Longitude': -86.0103, 'Elevation': 394},
        {'ID': 'KVOA', 'State': 'AL', 'Site': 'Viosca Knoll 786A', 'Latitude': 29.2289, 'Longitude': -87.7808, 'Elevation': 174},
        {'ID': 'MBLA1', 'State': 'AL', 'Site': 'Middle Bay Light', 'Latitude': 30.437, 'Longitude': -88.012, 'Elevation': 62},
        {'ID': 'MCGA1', 'State': 'AL', 'Site': 'Mobile/Coast Guard S', 'Latitude': 30.648, 'Longitude': -88.058, 'Elevation': 52},
        {'ID': 'MHPA1', 'State': 'AL', 'Site': 'Meaher Park', 'Latitude': 30.667, 'Longitude': -87.936, 'Elevation': 33},
        {'ID': 'OBLA1', 'State': 'AL', 'Site': 'Mobile State Docks', 'Latitude': 30.708, 'Longitude': -88.043, 'Elevation': 0},
        {'ID': 'PPTA1', 'State': 'AL', 'Site': 'Perdido Pass', 'Latitude': 30.279, 'Longitude': -87.556, 'Elevation': 16},
        {'ID': 'UNLA2', 'State': 'AL', 'Site': 'Unalaska', 'Latitude': 53.879, 'Longitude': -166.54, 'Elevation': 7},
        {'ID': 'WBYA1', 'State': 'AL', 'Site': 'Weeks Bay', 'Latitude': 30.417, 'Longitude': -87.825, 'Elevation': 0},
        {'ID': 'WKXA1', 'State': 'AL', 'Site': 'Weeks Bay Reserve', 'Latitude': 30.421, 'Longitude': -87.829, 'Elevation': 33}
    ]

    # Read the CSV file
    df = pd.read_csv(cleaned_csv_file)

    # Extract flight details from the first row of the CSV
    flight_date = datetime.strptime(df['Timestamp'].iloc[0], '%Y-%m-%dT%H:%M:%S.%fZ')
    tail_number = df['TailNumber'].iloc[0]
    departure_airport, arrival_airport = df['FlightTitle'].iloc[0].split(' - ')
    departure_coords = None

    # Dynamically generate the map_html filename using flight_date
    map_html = f"map_{flight_date.strftime('%m-%d-%Y')}.html"

    # Find the departure airport coordinates
    for airport in airport_coords:
        if airport['ID'] == departure_airport:
            departure_coords = (airport['Latitude'], airport['Longitude'])
            break

    # Calculate total flight time
    total_flight_time = calculate_flight_time(df)

    # Calculate night flight time
    total_night_time = 0  # Total night time in hours
    night_segments = []  # Track time spent during each night segment
    
    for i in range(1, len(df)):
        start_time = datetime.strptime(df['Timestamp'].iloc[i - 1], '%Y-%m-%dT%H:%M:%S.%fZ')
        end_time = datetime.strptime(df['Timestamp'].iloc[i], '%Y-%m-%dT%H:%M:%S.%fZ')
        segment_duration = (end_time - start_time).total_seconds() / 3600  # Duration in hours
        
        # Check if the segment falls in night hours
        if is_night_flight(start_time, df['Latitude'].iloc[i - 1], df['Longitude'].iloc[i - 1]):
            night_segments.append(segment_duration)
    total_night_time = round(sum(night_segments), 1)

    # Create a map centered around the first coordinate
    start_coords = (df['Latitude'][0], df['Longitude'][0])
    flight_map = folium.Map(location=start_coords, zoom_start=10)

    # Add the flight path to the map
    flight_path = list(zip(df['Latitude'], df['Longitude']))
    folium.PolyLine(flight_path, color='blue', weight=2.5, opacity=1).add_to(flight_map)

    # Add airport markers to the map and check for landings and low passes
    for airport in airport_coords:
        folium.Marker(location=(airport['Latitude'], airport['Longitude']), icon=folium.Icon(color='red', icon='plane')).add_to(flight_map)

    # Save the map to an HTML file
    flight_map.save(map_html)

    # Prepare to log landings and low passes
    remarks_list = []
    visited_airports = set() # Using set to account for unique airports
    intermediate_airports = []  # List to hold airports with any landings for route
    day_full_stop_landings = {}
    day_touch_and_go_landings = {}
    night_full_stop_landings = {}
    night_touch_and_go_landings = {}
    low_passes = {}
    last_landing_time = {}
    last_low_pass_time = {}
    route = []

    # Flags to track cross-country time qualification
    day_cross_country_all = False
    day_cross_country_50_nm = False
    night_cross_country_all = False
    night_cross_country_50_nm = False

    # Iterate through each data point in the flight path
    for i, (lat, lon, speed, altitude, timestamp) in enumerate(zip(df['Latitude'], df['Longitude'], df['Speed (knots)'], df['Altitude (feet)'], df['Timestamp'])):
        current_time = datetime.strptime(timestamp, '%Y-%m-%dT%H:%M:%S.%fZ')
        speed_flag = speed > 45  # Track if speed was above 45 knots previously

        # Find the closest airport
        closest_airport = None
        min_distance = float('inf')
        for airport in airport_coords:
            distance = haversine_final(lat, lon, airport['Latitude'], airport['Longitude'])
            if distance < min_distance:
                min_distance = distance
                closest_airport = airport

        # Check if the closest airport is within the landing threshold
        if min_distance <= 2 and speed < 50 and speed_flag and altitude < (closest_airport['Elevation'] + 100):
            # Check if a landing occurred
            airport_id = closest_airport['ID']
            visited_airports.add(airport_id)
            if airport_id not in last_landing_time or current_time - last_landing_time[airport_id] > timedelta(minutes=1):
                last_landing_time[airport_id] = current_time

                # Determine if it's a full stop or touch and go
                end_time = current_time + timedelta(minutes=1)
                full_stop = False
                for j in range(i, len(df)):
                    check_time = datetime.strptime(df['Timestamp'][j], '%Y-%m-%dT%H:%M:%S.%fZ')
                    if check_time > end_time:
                        break
                    if df['Speed (knots)'][j] < 10:
                        full_stop = True
                        break

                # Log day/night and landing type
                if is_night_landing(current_time, lat, lon):
                    if full_stop:
                        night_full_stop_landings[airport_id] = night_full_stop_landings.get(airport_id, 0) + 1
                    else:
                        night_touch_and_go_landings[airport_id] = night_touch_and_go_landings.get(airport_id, 0) + 1
                else:
                    if full_stop:
                        day_full_stop_landings[airport_id] = day_full_stop_landings.get(airport_id, 0) + 1
                    else:
                        day_touch_and_go_landings[airport_id] = day_touch_and_go_landings.get(airport_id, 0) + 1

                # Check cross-country eligibility and log intermediate airports
                if airport_id != departure_airport and airport_id != arrival_airport:
                    intermediate_airports.append(airport_id)
                    if not is_night_flight(current_time, lat, lon):
                        day_cross_country_all = True
                        if departure_coords and haversine_final(lat, lon, departure_coords[0], departure_coords[1]) >= 50:
                            day_cross_country_50_nm = True
                    if is_night_flight(current_time, lat, lon):
                        night_cross_country_all = True
                        if departure_coords and haversine_final(lat, lon, departure_coords[0], departure_coords[1]) >= 50:
                            night_cross_country_50_nm = True
        # Check for low passes
        if closest_airport and min_distance <= 2 and speed_flag and altitude < (airport['Elevation'] + 300) and altitude > (airport['Elevation'] + 50):
            airport_id = closest_airport['ID']
            if airport_id not in last_low_pass_time or current_time - last_low_pass_time[airport_id] > timedelta(minutes=5):
                low_passes[airport_id] = low_passes.get(airport_id, 0) + 1
                last_low_pass_time[airport_id] = current_time

    # Calculate totals for day and night landings
    total_day_landings = sum(day_full_stop_landings.values()) + sum(day_touch_and_go_landings.values())
    total_night_landings = sum(night_full_stop_landings.values()) + sum(night_touch_and_go_landings.values())

    # Checking if consecutive airports for airports along the route
    # Iterate over the recorded landings or airport visits
    for airport in intermediate_airports:
        # Add airport to visited set for unique airport count
        visited_airports.add(airport)
        
        # Avoid consecutive duplicate entries in the route list
        if not route or route[-1] != airport:
            route.append(airport)

    # Join route entries into the required format (e.g., "KSEM - KEET - KSEM")
    formatted_route = ' - '.join(route)

    # Calculate unique airports based on the visited_airports set
    unique_airports_visited = len(visited_airports)

    # Calculate cross-country times
    total_day_landings = sum(day_full_stop_landings.values()) + sum(day_touch_and_go_landings.values())
    total_night_landings = sum(night_full_stop_landings.values()) + sum(night_touch_and_go_landings.values())    
    day_cross_country_time_all = calculate_cross_country_time(total_flight_time, total_day_landings, unique_airports_visited, departure_airport, arrival_airport) if day_cross_country_all else 0
    day_cross_country_time_50_nm = calculate_cross_country_time(total_flight_time, total_day_landings, unique_airports_visited, departure_airport, arrival_airport) if day_cross_country_50_nm else 0
    night_cross_country_time_all = calculate_cross_country_time(total_flight_time, total_night_landings, unique_airports_visited, departure_airport, arrival_airport) if night_cross_country_all else 0
    night_cross_country_time_50_nm = calculate_cross_country_time(total_flight_time, total_night_landings, unique_airports_visited, departure_airport, arrival_airport) if night_cross_country_50_nm else 0

    # Detect steep turns
    steep_turns = detect_steep_turns(df)

    # Build remarks based on landings and low passes
    for airport in airport_coords:
        airport_id = airport['ID']
        if day_full_stop_landings.get(airport_id, 0) > 0:
            remarks_list.append(f"{airport_id}: {day_full_stop_landings[airport_id]} day full stop landings")
        if day_touch_and_go_landings.get(airport_id, 0) > 0:
            remarks_list.append(f"{airport_id}: {day_touch_and_go_landings[airport_id]} day touch and go landings")
        if night_full_stop_landings.get(airport_id, 0) > 0:
            remarks_list.append(f"{airport_id}: {night_full_stop_landings[airport_id]} night full stop landings")
        if night_touch_and_go_landings.get(airport_id, 0) > 0:
            remarks_list.append(f"{airport_id}: {night_touch_and_go_landings[airport_id]} night touch and go landings")
        if low_passes.get(airport_id, 0) > 0:
            remarks_list.append(f"{airport_id}: {low_passes[airport_id]} low passes")

    # Add steep turns to remarks
    if steep_turns > 0:
        remarks_list.append(f"Steep turns: {steep_turns}")

    # Combine remarks into a single string
    remarks = "; ".join(remarks_list)

    # Boolean-based flight time values
    airplane_single_time = total_flight_time if airplane_single else 0
    airplane_multi_time = total_flight_time if airplane_multi else 0
    pic_time = total_flight_time if pic else 0
    solo_time = total_flight_time if solo else 0

    # Save the map to an HTML file
    flight_map.save(map_html)

    # Write flight data to CSV with the correct format
    with open(output_csv, mode='a', newline='') as file:
        writer = csv.writer(file)

        # Write header only if the file is empty
        if file.tell() == 0:
            writer.writerow([
                'Date', 'Make/Model', 'Tail #', 'From', 'Route', 'To', 'Day Land', 'Night Land',
                '# Inst App', 'Type/Location Inst App', 'Airplane Single', 'Airplane Multi', 
                'Inst Act', 'Inst Sim/Hood', 'Inst FTD/Simulator',
                'Night', 'Day XC (All)', 'Day XC (>50 NM)', 
                'Night XC (All)', 'Night XC (>50 NM)', 'PIC', 'SOLO', 
                'Ground Train Received', 'Flight Train Received', 'Flight Train Given',
                'Total', 'Remarks'          
            ])

        writer.writerow([
            flight_date.strftime('%m/%d/%Y'),
            make_and_model,
            tail_number,
            departure_airport,
            formatted_route,
            arrival_airport,
            total_day_landings,
            total_night_landings,
            instrument_approach_num,
            instrument_approach_type_location,
            airplane_single_time,
            airplane_multi_time,
            instrument_actual_time,
            instrument_simulated_hood_time,
            instrument_simulator_ftd_time,
            total_night_time,
            day_cross_country_time_all,
            day_cross_country_time_50_nm,
            night_cross_country_time_all,
            night_cross_country_time_50_nm,
            pic_time,
            solo_time,
            ground_training_received_time,
            flight_training_received_time,
            flight_training_given_time,
            total_flight_time,
            remarks,
        ])


# Below is to run everything at once

def main(input_kml_file, make_and_model, instrument_approach_num, instrument_approach_type_location,
         airplane_single, airplane_multi, instrument_actual_time, instrument_simulated_hood_time,
         instrument_simulator_ftd_time, pic, solo, ground_training_received_time,
         flight_training_received_time, flight_training_given_time):
    
    input_csv_file = 'temp_input_csv_file.csv'
    cleaned_csv_file = 'temp_cleaned_csv_file.csv'
    
    # Process raw data from kml to csv
    kml_to_csv(input_kml_file, input_csv_file)
    
    # Process raw csv to cleaned csv
    calculate_speed_altitude_course(input_csv_file, cleaned_csv_file)

    # Process cleaned csv to logbook csv
    create_flight_path_map(
        cleaned_csv_file,
        make_and_model,
        instrument_approach_num,
        instrument_approach_type_location,
        airplane_single,
        airplane_multi,
        instrument_actual_time,
        instrument_simulated_hood_time,
        instrument_simulator_ftd_time,
        pic,
        solo,
        ground_training_received_time,
        flight_training_received_time,
        flight_training_given_time
    )

# # Example usage
# main(
#     input_kml_file='KML/TrackLog_E54708BF-CEEB-412E-B04F-CDF3C14E8D6D-2024.10.15.kml',
#     make_and_model='Cessna 172',
#     instrument_approach_num=2,
#     instrument_approach_type_location='ILS RWY 27 - KJFK',
#     airplane_single=True,
#     airplane_multi=False,
#     instrument_actual_time=1.5,
#     instrument_simulated_hood_time=0.5,
#     instrument_simulator_ftd_time=0.0,
#     pic=True,
#     solo=False,
#     ground_training_received_time=0.0,
#     flight_training_received_time=1.0,
#     flight_training_given_time=0.0
# )