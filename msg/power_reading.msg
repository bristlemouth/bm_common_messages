import sensor_header

# --- header ---
sensor_header header (this has several timestamps in it)

# --- type ---
uint8 power_reading_type # [source, load, monitor]
																# -- monitor => measuring power flowing through a BM node, but not providing or taking.
																# -- source => power into network. All readings from solar would be source.
																# -- load => power from network. All load monitors (eg 67) would be load, battery rechargeable would flip between load and source depending on state.
# --- Readings ---
float64 voltage_v
float64 current_ma
uint8 status              # --- okay, undervoltage, overvoltage, overcurrent, undercurrent (bit field)
