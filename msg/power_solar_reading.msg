import sensor_header

sensor_header header
uint8 power_reading_type
float64 voltage_v
float64 current_a
uint8 status
float64 mpp_position            # Where is power generation relative to Max Power Point of panel at current temp/age, as %.
float64 panel_temperatures[]    # Array of temperatures, one for each temp sensor in the panel/cell array
float64 panel_voltages[]        # Array of voltages, one for each isolated line in the array
float64 panel_currents[]        # Array of current, on for each isoltaed line in the array
