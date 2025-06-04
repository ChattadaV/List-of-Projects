import folium
import pandas as pd
import csv
from math import radians, cos, sin, sqrt, atan2, degrees
from datetime import datetime, timedelta
from astral.sun import sun
from astral import LocationInfo
import pytz

# Function to calculate the distance between two points using the Haversine formula
def haversine(lat1, lon1, lat2, lon2):
    R = 6371  # Radius of the Earth in kilometers
    dlat = radians(lat2 - lat1)
    dlon = radians(lon2 - lon1)
    a = sin(dlat / 2) ** 2 + cos(radians(lat1)) * cos(radians(lat2)) * sin(dlon / 2) ** 2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))
    distance = R * c
    return distance

# Function to determine if a landing is during the day or night
def is_night_landing(timestamp, latitude, longitude):
    location = LocationInfo(latitude=latitude, longitude=longitude)
    s = sun(location.observer, date=timestamp.date())
    sunset = s['sunset'] + timedelta(hours=1)
    sunrise = s['sunrise'] - timedelta(hours=1)
    
    # Convert timestamp to offset-aware datetime
    timestamp = timestamp.replace(tzinfo=pytz.UTC)
    return timestamp < sunrise or timestamp > sunset

# Function to calculate the bearing between two points
def calculate_bearing(lat1, lon1, lat2, lon2):
    dLon = radians(lon2 - lon1)
    lat1 = radians(lat1)
    lat2 = radians(lat2)
    x = sin(dLon) * cos(lat2)
    y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon)
    bearing = atan2(x, y)
    bearing = degrees(bearing)
    bearing = (bearing + 360) % 360
    return bearing

# # Function to detect steep turn
# def detect_steep_turn(df, index, radius_km=0.25):
#     center_lat, center_lon = df['Latitude'][index], df['Longitude'][index]
#     start_bearing = calculate_bearing(center_lat, center_lon, df['Latitude'][index+1], df['Longitude'][index+1])
#     bearings = [start_bearing]
#     for i in range(index+1, len(df)):
#         lat, lon = df['Latitude'][i], df['Longitude'][i]
#         distance = haversine(center_lat, center_lon, lat, lon)
#         if distance > radius_km:
#             return False, i
#         bearing = calculate_bearing(center_lat, center_lon, lat, lon)
#         bearings.append(bearing)
#         if len(bearings) > 1 and (bearings[-1] - bearings[0]) >= 360:
#             return True, i
#     return False, len(df)

# # Function to detect and count steep turns
# def detect_steep_turns(df, radius_km=0.25, stationary_threshold_km=0.1):
#     steep_turn_count = 0
    
#     for i in range(len(df) - 100):  # Use each point as a potential start of a circular path
#         start_lat = df['Latitude'][i]
#         start_lon = df['Longitude'][i]
#         completed_circle = False
#         max_distance = 0
#         is_stationary = True
        
#         # Check if the segment of points is stationary
#         for j in range(i + 1, i + 100):
#             next_lat = df['Latitude'][j]
#             next_lon = df['Longitude'][j]
#             if haversine(start_lat, start_lon, next_lat, next_lon) > stationary_threshold_km:
#                 is_stationary = False
#                 break

#         # Skip this segment if it's stationary
#         if is_stationary:
#             continue

#         # Check the next 50 points to see if they form a circle
#         for j in range(i + 10, i + 50):
#             current_lat = df['Latitude'][j]
#             current_lon = df['Longitude'][j]
            
#             # Check if we return to the starting point
#             if haversine(start_lat, start_lon, current_lat, current_lon) < 0.05:  # Small threshold to detect closure
#                 completed_circle = True
#                 path_segment = df.iloc[i:j+1]
#                 break

#         if not completed_circle:
#             continue

#         # Calculate center of path segment
#         center_lat = path_segment['Latitude'].mean()
#         center_lon = path_segment['Longitude'].mean()

#         # Check circularity by verifying that all points are within similar distance from the center
#         distances_to_center = [
#             haversine(center_lat, center_lon, row['Latitude'], row['Longitude'])
#             for _, row in path_segment.iterrows()
#         ]

#         # Calculate average distance and ensure deviations are minimal
#         avg_distance = sum(distances_to_center) / len(distances_to_center)
#         max_deviation = max(abs(d - avg_distance) for d in distances_to_center)

#         # Check that the path is circular: small deviation from average radius and consistent angular change
#         if max_deviation < radius_km * 0.1:  # Allow up to 10% deviation in radius
#             angles = []
#             for k in range(1, len(path_segment)):
#                 lat1, lon1 = path_segment.iloc[k-1][['Latitude', 'Longitude']]
#                 lat2, lon2 = path_segment.iloc[k][['Latitude', 'Longitude']]
#                 angle = atan2(lon2 - center_lon, lat2 - center_lat) - atan2(lon1 - center_lon, lat1 - center_lat)
#                 angle = degrees(angle)
#                 angles.append(angle if angle >= 0 else angle + 360)

#             # Smooth progression of angles indicates circular path
#             if all(angles[k] > angles[k-1] for k in range(1, len(angles))) or all(angles[k] < angles[k-1] for k in range(1, len(angles))):
#                 steep_turn_count += 1
#                 i += 100  # Skip ahead to avoid recounting the same maneuver
    
#     return steep_turn_count

# Function to calculate total flight time in hours with 1 decimal point
def calculate_flight_time(df):
    start_time = datetime.strptime(df['Timestamp'].iloc[0], '%Y-%m-%dT%H:%M:%S.%fZ')
    end_time = datetime.strptime(df['Timestamp'].iloc[-1], '%Y-%m-%dT%H:%M:%S.%fZ')
    total_duration = (end_time - start_time).total_seconds() / 3600  # Convert seconds to hours
    return round(total_duration, 1)

# Function to create a map with the flight path and log landings, low passes, flight time
# def create_flight_path_map(csv_file, airport_coords, output_html, landings_csv, maneuvers_csv, buffer_radius_km=0.5):
def create_flight_path_map(csv_file, airport_coords, output_html, landings_csv, buffer_radius_km=0.5):

    # Read the CSV file
    df = pd.read_csv(csv_file)
    # Create a map centered around the first coordinate
    start_coords = (df['Latitude'][0], df['Longitude'][0])
    flight_map = folium.Map(location=start_coords, zoom_start=10)
    # Add the flight path to the map
    flight_path = list(zip(df['Latitude'], df['Longitude']))
    folium.PolyLine(flight_path, color='blue', weight=2.5, opacity=1).add_to(flight_map)
    
    # Prepare to log landings and low passes
    landings = []
    last_landing_time = {}
    day_full_stop_landings = {}
    day_touch_and_go_landings = {}
    night_full_stop_landings = {}
    night_touch_and_go_landings = {}
    total_day_landings = {}
    total_night_landings = {}
    low_passes = {}
    last_low_pass_time = {}
    
    # Add airport markers to the map and check for landings and low passes
    for airport in airport_coords:
        folium.Marker(location=(airport['Latitude'], airport['Longitude']), icon=folium.Icon(color='red', icon='plane')).add_to(flight_map)
        speed_flag = False  # Initialize the speed flag
        for i, (lat, lon, speed, altitude, timestamp) in enumerate(zip(df['Latitude'], df['Longitude'], df['Speed (knots)'], df['Altitude (feet)'], df['Timestamp'])):
            distance = haversine(lat, lon, airport['Latitude'], airport['Longitude'])
            if speed > 45 and speed_flag == False:
                speed_flag = True  # Set the flag to true when speed is above 30 knots
            if distance <= buffer_radius_km and speed < 30 and speed_flag and altitude < (airport['Elevation'] + 100):
                current_time = datetime.strptime(timestamp, '%Y-%m-%dT%H:%M:%S.%fZ')
                if (airport['ID'] not in last_landing_time or current_time - last_landing_time[airport['ID']] > timedelta(minutes=5)) and speed_flag:
                    landings.append(airport)
                    last_landing_time[airport['ID']] = current_time
                    # Check speed for the next 1 minute
                    end_time = current_time + timedelta(minutes=1)
                    full_stop = False
                    for j in range(i, len(df)):
                        check_time = datetime.strptime(df['Timestamp'][j], '%Y-%m-%dT%H:%M:%S.%fZ')
                        if check_time > end_time:
                            break
                        if df['Speed (knots)'][j] < 10:
                            full_stop = True
                            break
                    if full_stop:
                        if is_night_landing(current_time, lat, lon):
                            night_full_stop_landings[airport['ID']] = night_full_stop_landings.get(airport['ID'], 0) + 1
                            speed_flag = False  # Reset the flag when landing is detected
                        else:
                            day_full_stop_landings[airport['ID']] = day_full_stop_landings.get(airport['ID'], 0) + 1
                            speed_flag = False  # Reset the flag when landing is detected
                    else:
                        if is_night_landing(current_time, lat, lon):
                            night_touch_and_go_landings[airport['ID']] = night_touch_and_go_landings.get(airport['ID'], 0) + 1
                            speed_flag = False  # Reset the flag when landing is detected
                        else:
                            day_touch_and_go_landings[airport['ID']] = day_touch_and_go_landings.get(airport['ID'], 0) + 1
                            speed_flag = False  # Reset the flag when landing is detected
                    if is_night_landing(current_time, lat, lon):
                        total_night_landings[airport['ID']] = total_night_landings.get(airport['ID'], 0) + 1
                        speed_flag = False  # Reset the flag when landing is detected
                    else:
                        total_day_landings[airport['ID']] = total_day_landings.get(airport['ID'], 0) + 1
                        speed_flag = False  # Reset the flag when landing is detected
            # Check for low pass
            if distance <= buffer_radius_km and speed < 60 and speed_flag and altitude < (airport['Elevation'] + 300) and altitude > (airport['Elevation'] + 50):
                current_time = datetime.strptime(timestamp, '%Y-%m-%dT%H:%M:%S.%fZ')
                if airport['ID'] not in last_low_pass_time or current_time - last_low_pass_time[airport['ID']] > timedelta(minutes=5):
                    # Check if the flight path segment is a straight line
                    if i > 0 and i < len(df) - 1:
                        prev_lat, prev_lon = df['Latitude'][i - 10], df['Longitude'][i - 10]
                        next_lat, next_lon = df['Latitude'][i + 10], df['Longitude'][i + 10]
                        bearing1 = calculate_bearing(prev_lat, prev_lon, lat, lon)
                        bearing2 = calculate_bearing(lat, lon, next_lat, next_lon)
                        if abs(bearing1 - bearing2) < 5:  # Allow small deviation for straight line
                            low_passes[airport['ID']] = low_passes.get(airport['ID'], 0) + 1
                            last_low_pass_time[airport['ID']] = current_time
    
    # # Detect steep turns and write them to a new CSV file
    # steep_turn_count = detect_steep_turns(df)
    # with open(maneuvers_csv, mode='w', newline='') as file:
    #     writer = csv.writer(file)
    #     writer.writerow(['Steep Turns'])
    #     writer.writerow([steep_turn_count])

    # Calculate total flight time
    total_flight_time = calculate_flight_time(df)
    
    # Save the map to an HTML file
    flight_map.save(output_html)
    
    # Log landings and low passes to a CSV file
    landing_counts = {}
    for landing in landings:
        landing_counts[landing['ID']] = landing_counts.get(landing['ID'], 0) + 1
    with open(landings_csv, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['ID', 'State', 'Site', 'Total Landings', 'Day Full Stop Landings', 'Day Touch & Go Landings', 'Night Full Stop Landings', 'Night Touch & Go Landings', 'Total Day Landings', 'Total Night Landings', 'Low Passes', 'Total Flight Time'])
        for airport in airport_coords:
            total_landings = landing_counts.get(airport['ID'], 0)
            day_full_stop_count = day_full_stop_landings.get(airport['ID'], 0)
            day_touch_and_go_count = day_touch_and_go_landings.get(airport['ID'], 0)
            night_full_stop_count = night_full_stop_landings.get(airport['ID'], 0)
            night_touch_and_go_count = night_touch_and_go_landings.get(airport['ID'], 0)
            total_day_count = total_day_landings.get(airport['ID'], 0)
            total_night_count = total_night_landings.get(airport['ID'], 0)
            low_pass_count = low_passes.get(airport['ID'], 0)
            if total_landings > 0 or low_pass_count > 0:
                writer.writerow([airport['ID'], airport['State'], airport['Site'], total_landings, day_full_stop_count, day_touch_and_go_count, night_full_stop_count, night_touch_and_go_count, total_day_count, total_night_count, low_pass_count, total_flight_time])



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


csv_file = 'output_from_gps_pitch_bank_2024.08.23.csv'
output_html = 'flight_path_map_2024.08.23.html'
visited_airports_csv = 'visited_airport_2024.08.23.csv'
# maneuvers_csv = 'maneuvers_2024.08.23.csv'

# csv_file = 'csv_cleaned_2024.10.09.csv'
# output_html = 'flight_path_map_2024.10.09.html'
# visited_airports_csv = 'visited_airport_2024.10.09.csv'
# maneuvers_csv = 'maneuvers_2024.10.09.csv'
# create_flight_path_map(csv_file, airport_coords, output_html, visited_airports_csv, maneuvers_csv)
create_flight_path_map(csv_file, airport_coords, output_html, visited_airports_csv)

