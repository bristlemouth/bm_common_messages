import power_reading

# --- Imported types ---
sensor_header header
power_reading reading

# --- battery specifics ---

# --- State of Charge SOC ---
float64 charge           # Current charge in Ah  (If unmeasured NaN)
float64 capacity         # Capacity in Ah (last full capacity)  (If unmeasured NaN)
float64 percentage       # Charge percentage on 0 to 1 range  (If unmeasured NaN)

# --- Status / health / info ---
uint8 power_supply_status     # The charging status as reported. Values defined above
uint8 power_supply_health     # The battery health metric. Values defined above

# --- Cell Data ---
float64 cell_voltage[]      # An array of individual cell voltages for each cell in the pack
				                         # If individual voltages unknown but number of cells known set each to NaN
float64 cell_temperature[]  # An array of individual cell temperatures for each cell in the pack
			                           # If individual temperatures unknown but number of cells known set each to NaN
