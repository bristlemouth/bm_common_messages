import power_reading_averages

# --- Imported types ---
sensor_header header
power_reading_averages reading_averages

# --- battery specifics ---

# --- Cell Data ---
float64 cell_voltage_average[]      # An array of averaged individual cell voltages for each cell in the pack
				                         # If individual voltages unknown but number of cells known set each to NaN
float64 cell_voltage_max[]   # An array of individual cell voltage max's for each cell in the pack
				                         # If individual voltages unknown but number of cells known set each to NaN
float64 cell_voltage_min[]   # An array of individual cell voltage max's for each cell in the pack
				                         # If individual voltages unknown but number of cells known set each to NaN
float64 cell_voltage_stdev[]   # An array of individual cell voltage stdev's for each cell in the pack
				                         # If individual voltages unknown but number of cells known set each to NaN
float64 cell_temperature_average[]  # An array of averaged individual cell temperatures for each cell in the pack
			                           # If individual temperatures unknown but number of cells known set each to NaN
float64 cell_temperature_max[]  # An array of averaged individual cell temperature max's for each cell in the pack
			                           # If individual temperatures unknown but number of cells known set each to NaN
float64 cell_temperature_min[]  # An array of averaged individual cell temperature min's for each cell in the pack
			                           # If individual temperatures unknown but number of cells known set each to NaN
float64 cell_temperature_stdev[]  # An array of averaged individual cell temperature stdevs for each cell in the pack
			                           # If individual temperatures unknown but number of cells known set each to NaN
