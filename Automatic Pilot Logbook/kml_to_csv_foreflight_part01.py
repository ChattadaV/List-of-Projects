import xml.etree.ElementTree as ET
import csv

# Function to extract data from KML file and save to CSV
def kml_to_csv(kml_file, csv_file):
    # Namespaces used in the KML file
    ns = {'kml': 'http://www.opengis.net/kml/2.2', 'gx': 'http://www.google.com/kml/ext/2.2'}

    # Parse the KML file
    tree = ET.parse(kml_file)
    root = tree.getroot()

    # Prepare headers for the CSV file
    headers = ['Timestamp', 'Longitude', 'Latitude', 'Altitude', 'Horizontal Acceleration', 'Vertical Acceleration', 'Course', 'Speed (Kts)', 'Altitude2', 'Bank', 'Pitch', 'Source', 'GPSModelName', 'FlightTitle', 'PilotName', 'TailNumber', 'PilotNotes', 'RouteWaypoints']

    # Open CSV file for writing
    with open(csv_file, mode='w', newline='') as file:
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
kml_to_csv('TrackLog_742441B6-155D-4F90-A472-4394FB0AB423.kml', 'output.csv')
