import os
import io
import streamlit as st
import pandas as pd
import folium

# Import the backend function
from back_end_final import main

# Ensure the temp directory exists
if not os.path.exists("temp"):
    os.makedirs("temp")

# Streamlit app
st.title("Automatic Pilot Logbook")

# Input fields
kml_files = st.file_uploader("Upload KML Files", type=["kml"], accept_multiple_files=True)

# Create three columns for input fields
col1, col2, col3 = st.columns(3)

with col1:
    make_and_model = st.text_input("Make and Model", "PA28-150")
    instrument_approach_num = st.number_input("Number of Instrument App", min_value=0, value=0, format="%d", step=1)
    instrument_approach_type_location = st.text_input("Type & Location of Instrument App", "N/A")
    airplane_type = st.radio("Airplane Type", ("Single", "Multi"))

with col2:
    instrument_actual_time = st.number_input("Instrument Actual Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)
    instrument_simulated_hood_time = st.number_input("Instrument Simulated (Hood) Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)
    instrument_simulator_ftd_time = st.number_input("Instrument FTD/Simulator Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)
    pic = st.checkbox("PIC", value=True)
    solo = st.checkbox("Solo", value=True)

with col3:
    ground_training_received_time = st.number_input("Ground Training Received Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)
    flight_training_received_time = st.number_input("Flight Training Received Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)
    flight_training_given_time = st.number_input("Flight Training Given Time", min_value=0.0, value=0.0, format="%.1f", step=0.1)

# Initialize session state for CSV data
if "csv_data" not in st.session_state:
    st.session_state.csv_data = None

if st.button("Process Data"):
    if kml_files is not None:
        try:
            all_data = []
            for kml_file in kml_files:
                st.write(f"Uploading file {kml_file.name}...")
                # Save the uploaded file
                input_kml_path = os.path.join("temp", kml_file.name)
                with open(input_kml_path, "wb") as f:
                    f.write(kml_file.getbuffer())
                st.write(f"File {kml_file.name} uploaded successfully.")

                # Call the backend function
                st.write(f"Processing file {kml_file.name}...")
                main(
                    input_kml_file=input_kml_path,
                    make_and_model=make_and_model,
                    instrument_approach_num=instrument_approach_num,
                    instrument_approach_type_location=instrument_approach_type_location,
                    airplane_single=airplane_type == "Single",
                    airplane_multi=airplane_type == "Multi",
                    instrument_actual_time=instrument_actual_time,
                    instrument_simulated_hood_time=instrument_simulated_hood_time,
                    instrument_simulator_ftd_time=instrument_simulator_ftd_time,
                    pic=pic,
                    solo=solo,
                    ground_training_received_time=ground_training_received_time,
                    flight_training_received_time=flight_training_received_time,
                    flight_training_given_time=flight_training_given_time
                )
                st.write(f"File {kml_file.name} processed successfully.")

                # Read the processed CSV data and append to the list
                output_df = pd.read_csv("pilot_logbook.csv")
                all_data.append(output_df)

            # Combine all data into a single DataFrame
            combined_df = pd.concat(all_data, ignore_index=True)

            # Store combined data in session state
            st.session_state.csv_data = combined_df.to_csv(index=False)

        except Exception as e:
            st.error(f"An error occurred: {e}")
    else:
        st.error("Please upload KML files.")

# Display and edit CSV data if available
if st.session_state.csv_data:
    st.subheader("Data Verification - 5 Most Recent Flights")
    combined_df = pd.read_csv(io.StringIO(st.session_state.csv_data))
    edited_df = st.data_editor(combined_df.tail(5))

    if st.button("Submit Changes"):
        try:
            st.write("Saving changes to CSV file...")
            edited_df.to_csv("pilot_logbook.csv", index=False)
            st.success("Changes saved successfully.")
            st.write("Updated CSV Data:")
            st.dataframe(edited_df.tail(5))

            # Update session state with new CSV data
            st.session_state.csv_data = edited_df.to_csv(index=False)
        except Exception as e:
            st.error(f"An error occurred while saving changes: {e}")

    # Display the HTML map
    st.subheader("Interactive Map")
    map_html = f"map_{pd.to_datetime(pd.read_csv(io.StringIO(st.session_state.csv_data))['Date'].iloc[-1]).strftime('%m-%d-%Y')}.html"
    with open(map_html, "r") as f:
        st.components.v1.html(f.read(), height=600)