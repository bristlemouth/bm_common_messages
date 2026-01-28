import power_reading_averages

# --- Imported types ---
sensor_header header
power_reading_averages reading_averages

# --- Solar Specifics ---
float64 panel_temperatures_average[]    # Array of averaged temperatures, one for each temp sensor in the panel/cell array
float64 panel_temperatures_max[]        # Array of max temperatures, one for each temp sensor in the panel/cell array
float64 panel_temperatures_min[]        # Array of min temperatures, one for each temp sensor in the panel/cell array
float64 panel_temperatures_stdev[]      # Array of stdev temperatures, one for each temp sensor in the panel/cell array
float64 panel_voltages_average[]        # Array of averaged voltages, one for each isolated line in the array
float64 panel_voltages_max[]            # Array of max voltages, one for each isolated line in the array
float64 panel_voltages_min[]            # Array of min voltages, one for each isolated line in the array
float64 panel_voltages_stdev[]          # Array of stdev voltages, one for each isolated line in the array
float64 panel_currents_average[]        # Array of average currents, one for each isolated line in the array
float64 panel_currents_max[]            # Array of max currents, one for each isolated line in the array
float64 panel_currents_min[]            # Array of min currents, one for each isolated line in the array
float64 panel_currents_stdev[]          # Array of stdev currents, one for each isolated line in the array