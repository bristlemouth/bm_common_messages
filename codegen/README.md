# CBOR Message Code Generation

This directory contains tools to automatically generate C17-compatible CBOR encoder/decoder code from ROS-style `.msg` definition files.

## Usage

### 1. Preview Generated Code (Safe)

**Python direct:**
```bash
python3 c_generator.py ../msg/
# Outputs to current directory
```

**Python script (recommended):**
```bash
python3 generate_and_replace.py --preview
# Outputs to codegen/preview/
```

**CMake target:**
```bash
cmake --build . --target generate_cbor_messages_preview
# Outputs to codegen/preview/
```

### 2. Replace Existing Files (Manual Override)

⚠️ **WARNING**: This will overwrite existing handwritten implementations!

```bash
# With confirmation prompt
python3 generate_and_replace.py --replace

# Skip confirmation (dangerous)
python3 generate_and_replace.py --replace --force
```

## What Gets Generated

For each `.msg` file, the generator creates:

- **`<message_name>_msg.h`** - Header with struct definitions and function declarations
- **`<message_name>_msg.c`** - Implementation with CBOR encode/decode functions

### Generated Code Features

✅ **C17-compatible** - Strict C17 standard compliance
✅ **Low-level API** - Works directly with `CborEncoder*` parameters
✅ **Proper error handling** - Matches existing CBOR patterns
✅ **Complex types** - Handles strings, arrays, optional fields
✅ **Dependencies** - Automatically resolves message imports

## Message Definition Format

See `../msg/` directory for examples. Format:
```
import sensor_header

sensor_header header
float64 temperature_deg_c
optional float64 pressure_bar
uint32 sensor_id
string device_name
array cbor_data
```

## Safety Features

- **Never runs automatically** - All generation is manual/opt-in
- **Preview mode default** - Safe preview directory by default
- **Confirmation prompts** - Requires explicit confirmation to overwrite
- **Git-friendly** - Easy to diff and review changes

## Files

- `msg_parser.py` - Parses .msg definition files
- `c_generator.py` - Generates C17 CBOR code
- `generate_and_replace.py` - User-friendly generation script
- `GenerateMessages.cmake` - CMake integration (optional target only)