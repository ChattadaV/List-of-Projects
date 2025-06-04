import webbrowser  # To open HTML file in default browser

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.filechooser import FileChooserListView
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.checkbox import CheckBox
from kivy.uix.button import Button
from kivy.uix.popup import Popup
from kivy.properties import StringProperty, NumericProperty, BooleanProperty

import csv
import csv_to_data_formatted  # Your backend file



class MainScreen(BoxLayout):
    def __init__(self, **kwargs):
        super(MainScreen, self).__init__(**kwargs)
        self.orientation = 'vertical'
        self.padding = 10
        self.spacing = 10

        # Pre-define output paths
        self.map_html = "map_html.html"
        self.pilot_logbook = "pilot_logbook.csv"

        # Top-level layout with 2 columns
        layout = BoxLayout(orientation='horizontal', spacing=10)

        # Left column
        left_col = BoxLayout(orientation='vertical', spacing=10)

        # Right column
        right_col = BoxLayout(orientation='vertical', spacing=10)

        # File chooser button
        self.csv_file = StringProperty("")
        browse_button = Button(text="Browse for File")
        browse_button.bind(on_release=self.open_filechooser)
        left_col.add_widget(Label(text="Select CSV File"))
        left_col.add_widget(browse_button)

        # Input fields organized in rows
        self.make_and_model = TextInput(hint_text="Make and Model", multiline=False)
        left_col.add_widget(Label(text="Make and Model"))
        left_col.add_widget(self.make_and_model)

        self.instrument_approach_num = TextInput(hint_text="Instrument Approach Number", input_filter='int', multiline=False)
        right_col.add_widget(Label(text="Instrument Approach Number"))
        right_col.add_widget(self.instrument_approach_num)

        self.instrument_approach_type_location = TextInput(hint_text="Instrument Approach Type/Location", multiline=False)
        left_col.add_widget(Label(text="Instrument Approach Type/Location"))
        left_col.add_widget(self.instrument_approach_type_location)

        # Boolean fields as checkboxes
        self.airplane_single = CheckBox()
        right_col.add_widget(Label(text="Single-engine airplane"))
        right_col.add_widget(self.airplane_single)

        self.airplane_multi = CheckBox()
        left_col.add_widget(Label(text="Multi-engine airplane"))
        left_col.add_widget(self.airplane_multi)

        # Float input fields
        self.instrument_actual_time = TextInput(hint_text="Instrument Actual Time", input_filter='float', multiline=False)
        right_col.add_widget(Label(text="Instrument Actual Time"))
        right_col.add_widget(self.instrument_actual_time)

        self.instrument_simulated_hood_time = TextInput(hint_text="Instrument Simulated Hood Time", input_filter='float', multiline=False)
        left_col.add_widget(Label(text="Instrument Simulated Hood Time"))
        left_col.add_widget(self.instrument_simulated_hood_time)

        self.instrument_simulator_ftd_time = TextInput(hint_text="Instrument Simulator/FTD Time", input_filter='float', multiline=False)
        right_col.add_widget(Label(text="Instrument Simulator/FTD Time"))
        right_col.add_widget(self.instrument_simulator_ftd_time)

        # Additional Boolean fields as checkboxes
        self.pic = CheckBox()
        left_col.add_widget(Label(text="Pilot in Command"))
        left_col.add_widget(self.pic)

        self.solo = CheckBox()
        right_col.add_widget(Label(text="Solo"))
        right_col.add_widget(self.solo)

        # Additional float input fields
        self.ground_training_received_time = TextInput(hint_text="Ground Training Received Time", input_filter='float', multiline=False)
        left_col.add_widget(Label(text="Ground Training Received Time"))
        left_col.add_widget(self.ground_training_received_time)

        self.flight_training_received_time = TextInput(hint_text="Flight Training Received Time", input_filter='float', multiline=False)
        right_col.add_widget(Label(text="Flight Training Received Time"))
        right_col.add_widget(self.flight_training_received_time)

        self.flight_training_given_time = TextInput(hint_text="Flight Training Given Time", input_filter='float', multiline=False)
        left_col.add_widget(Label(text="Flight Training Given Time"))
        left_col.add_widget(self.flight_training_given_time)

        # Add columns to the main layout
        layout.add_widget(left_col)
        layout.add_widget(right_col)
        self.add_widget(layout)

        # Generate button at the bottom
        self.generate_button = Button(text="Generate Output", size_hint=(1, 0.1))
        self.generate_button.bind(on_release=self.generate_output)
        self.add_widget(self.generate_button)

    def open_filechooser(self, instance):
        # FileChooser popup for selecting CSV file
        filechooser_layout = BoxLayout(orientation='vertical')
        content = FileChooserListView()
        select_button = Button(text="Select File", size_hint=(1, 0.1))

        filechooser_layout.add_widget(content)
        filechooser_layout.add_widget(select_button)

        # Popup with a selectable file chooser
        popup = Popup(title="Select CSV File", content=filechooser_layout, size_hint=(0.9, 0.9))

        select_button.bind(on_release=lambda x: self.set_file(popup, content.selection))
        popup.open()

    def set_file(self, popup, selection):
        if selection:
            self.csv_file = selection[0]
            popup.dismiss()

    def generate_output(self, instance):
        try:
            # Call the backend function with the current values
            csv_to_data_formatted.create_flight_path_map(
                input_csv=self.csv_file,          
                make_and_model=self.make_and_model.text,
                instrument_approach_num=int(self.instrument_approach_num.text) if self.instrument_approach_num.text else 0,
                instrument_approach_type_location=self.instrument_approach_type_location.text,
                airplane_single=self.airplane_single.active,
                airplane_multi=self.airplane_multi.active,
                instrument_actual_time=float(self.instrument_actual_time.text) if self.instrument_actual_time.text else 0.0,
                instrument_simulated_hood_time=float(self.instrument_simulated_hood_time.text) if self.instrument_simulated_hood_time.text else 0.0,
                instrument_simulator_ftd_time=float(self.instrument_simulator_ftd_time.text) if self.instrument_simulator_ftd_time.text else 0.0,
                pic=self.pic.active,
                solo=self.solo.active,
                ground_training_received_time=float(self.ground_training_received_time.text) if self.ground_training_received_time.text else 0.0,
                flight_training_received_time=float(self.flight_training_received_time.text) if self.flight_training_received_time.text else 0.0,
                flight_training_given_time=float(self.flight_training_given_time.text) if self.flight_training_given_time.text else 0.0
            )
            # self.show_popup("Output generated successfully!")
            # Show popup with HTML map and latest CSV row
            self.show_output_popup()
        except Exception as e:
            self.show_popup(f"Error: {e}")

    def show_output_popup(self):
        # Open HTML map in default browser
        webbrowser.open(self.map_html)
        
        # Layout for the popup content
        popup_layout = BoxLayout(orientation='vertical', spacing=10)

        # Load the latest row from CSV
        try:
            latest_row = self.get_latest_csv_row()
            latest_row_label = Label(text=str(latest_row), size_hint=(1, 1))
        except Exception as e:
            latest_row_label = Label(text=f"Error loading CSV: {e}", size_hint=(1, 1))

        popup_layout.add_widget(latest_row_label)

        # Popup with map and recent CSV data
        popup = Popup(title="Generated Output", content=popup_layout, size_hint=(0.9, 0.3))
        popup.open()

    def get_latest_csv_row(self):

        with open(self.pilot_logbook, mode='r') as csv_file:

            csv_reader = csv.reader(csv_file)

            rows = list(csv_reader)

            return rows[-1] if rows else ["No data found"]
        
    def show_popup(self, message):
        popup = Popup(title="Message",
                      content=Label(text=message),
                      size_hint=(0.6, 0.4),
                      auto_dismiss=True)
        popup.open()

class FlightLoggerApp(App):
    def build(self):
        return MainScreen()

if __name__ == "__main__":
    FlightLoggerApp().run()
