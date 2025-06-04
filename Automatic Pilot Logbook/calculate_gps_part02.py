import csv
import math

def haversine(lon1, lat1, lon2, lat2):
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
        fieldnames = ['Timestamp', 'Speed (knots)', 'Altitude (feet)', 'Course (degrees)', 'Latitude', 'Longitude', 'Source', 'GPSModelName', 'FlightTitle']
        writer = csv.DictWriter(outfile, fieldnames=fieldnames)
        writer.writeheader()
        
        previous_row = None
        for row in reader:
            if previous_row:
                lon1, lat1, alt1 = float(previous_row['Longitude']), float(previous_row['Latitude']), float(previous_row['Altitude'])
                lon2, lat2, alt2 = float(row['Longitude']), float(row['Latitude']), float(row['Altitude'])
                distance = haversine(lon1, lat1, lon2, lat2)
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
                    'Source': row['Source'],
                    'GPSModelName': row['GPSModelName'],
                    'FlightTitle': row['FlightTitle']
                })
            previous_row = row

input_file = 'output.csv'
output_file = 'output_from_gps_pitch_bank.csv'
calculate_speed_altitude_course(input_file, output_file)
