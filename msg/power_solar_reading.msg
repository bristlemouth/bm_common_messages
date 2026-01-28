import power_reading

# --- Imported types ---
sensor_header header
power_reading reading

# --- Solar Specifics ---
float64 mpp_position            # Where is power generation relative to Max Power Point of panel at current temp/age, as %.
float64 panel_temperatures[]    # Array of temperatures, one for each temp sensor in the panel/cell array
float64 panel_voltages[]        # Array of voltages, one for each isolated line in the array
float64 panel_currents[]        # ditto