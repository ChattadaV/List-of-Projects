import xml.etree.ElementTree as ET
import csv
import math

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
                distance_meter = haversine_initial(lon1, lat1, lon2, lat2)
                speed_mps = distance_meter  # Speed in meters per second (since data is logged every second)
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

# Usage
# Change KML file name to be closest to current time format or look at the latest modified status
# Change output CSV file name to reflect date and time
# Add another output CSV to represents automatic pilot logbook
# kml_input_file = 'KML/TrackLog_B5B519C7-0326-46A1-A09D-1B389690E37E-2024.10.09.kml'
# csv_output_file = 'csv_2024.10.09.csv'
# kml_input_file = 'KML/TrackLog_E54708BF-CEEB-412E-B04F-CDF3C14E8D6D-2024.10.15.kml'
# csv_output_file = 'csv_2024.10.15.csv'
# kml_to_csv(kml_input_file, csv_output_file)

# input_file = csv_output_file
# output_file_final = 'csv_cleaned_2024.10.09.csv'
# # output_file_final = 'csv_cleaned_2024.10.15.csv'
# calculate_speed_altitude_course(input_file, output_file_final)
