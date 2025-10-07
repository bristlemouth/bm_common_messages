#!/usr/bin/env python3

import argparse
import os
import sys
from dataclasses import dataclass
from typing import List


@dataclass
class Field:
    """Represents a field in a message"""

    type_name: str
    field_name: str
    is_primitive: bool


@dataclass
class Message:
    """Represents a parsed ROS message"""

    name: str
    imports: List[str]
    fields: List[Field]


def parse_msg_file(file_path: str) -> Message:
    """Parse a ROS .msg file and extract structure"""
    imports: List[str] = []
    fields: List[Field] = []

    # Primitive type mappings
    primitives = {
        "bool",
        "int8",
        "uint8",
        "int16",
        "uint16",
        "int32",
        "uint32",
        "int64",
        "uint64",
        "float32",
        "float64",
    }

    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()

            # Skip empty lines and comments
            if not line or line.startswith("#"):
                continue

            # Handle imports
            if line.startswith("import "):
                import_name = line.split()[1]
                imports.append(import_name)
                continue

            # Handle field definitions
            parts = line.split()
            if len(parts) >= 2:
                type_name = parts[0]
                field_name = parts[1]
                is_primitive = type_name in primitives

                fields.append(
                    Field(
                        type_name=type_name,
                        field_name=field_name,
                        is_primitive=is_primitive,
                    )
                )

    # Extract message name from file path
    msg_name = os.path.splitext(os.path.basename(file_path))[0]

    return Message(name=msg_name, imports=imports, fields=fields)


def ros_type_to_c_type(ros_type: str) -> str:
    """Convert ROS type to C17 type"""
    type_map = {
        "bool": "bool",
        "int8": "int8_t",
        "uint8": "uint8_t",
        "int16": "int16_t",
        "uint16": "uint16_t",
        "int32": "int32_t",
        "uint32": "uint32_t",
        "int64": "int64_t",
        "uint64": "uint64_t",
        "float32": "float",
        "float64": "double",
    }
    return type_map.get(ros_type, ros_type + "_t")


def generate_header(message: Message, output_dir: str) -> str:
    """Generate C17 header file"""
    header_name = f"{message.name}_msg.h"
    header_path = os.path.join(output_dir, header_name)
    guard_name = f"{message.name.upper()}_MSG_H"

    with open(header_path, "w") as f:
        # Header guard and includes
        f.write(f"#ifndef {guard_name}\n")
        f.write(f"#define {guard_name}\n\n")
        f.write('#include "cbor.h"\n')

        # Include headers for nested messages
        for imp in message.imports:
            f.write(f'#include "{imp}_msg.h"\n')

        f.write("\n")

        # Struct definition
        f.write(f"typedef struct {{\n")
        for field in message.fields:
            c_type = ros_type_to_c_type(field.type_name)
            f.write(f"    {c_type} {field.field_name};\n")
        f.write(f"}} {message.name}_t;\n\n")

        # Function declarations
        f.write(f"// Encode {message.name} to CBOR\n")
        f.write(
            f"CborError {message.name}_encode(CborEncoder *encoder, const {message.name}_t *msg);\n\n"
        )

        f.write(f"// Decode {message.name} from CBOR\n")
        f.write(
            f"CborError {message.name}_decode(CborValue *value, {message.name}_t *msg);\n\n"
        )

        f.write(f"#endif // {guard_name}\n")

    return header_path


def generate_encode_field(field: Field, encoder_name: str, indent: str = "    ") -> str:
    """Generate encoding code for a single field"""
    code: List[str] = []
    var_name = f"msg->{field.field_name}"

    if field.is_primitive:
        type_name = field.type_name
        if type_name == "bool":
            code.append(
                f"{indent}err = cbor_encode_boolean({encoder_name}, {var_name});"
            )
        elif type_name in ["int8", "int16", "int32", "int64"]:
            code.append(f"{indent}err = cbor_encode_int({encoder_name}, {var_name});")
        elif type_name in ["uint8", "uint16", "uint32", "uint64"]:
            code.append(f"{indent}err = cbor_encode_uint({encoder_name}, {var_name});")
        elif type_name in ["float32", "float64"]:
            code.append(
                f"{indent}err = cbor_encode_double({encoder_name}, {var_name});"
            )
    else:
        # Nested message
        code.append(
            f"{indent}err = {field.type_name}_encode({encoder_name}, &{var_name});"
        )

    code.append(f"{indent}if (err != CborNoError) return err;")
    return "\n".join(code)


def generate_decode_field(field: Field, value_name: str, indent: str = "    ") -> str:
    """Generate decoding code for a single field"""
    code: List[str] = []
    var_name = f"msg->{field.field_name}"

    if field.is_primitive:
        type_name = field.type_name
        if type_name == "bool":
            code.append(
                f"{indent}err = cbor_value_get_boolean({value_name}, &{var_name});"
            )
        elif type_name in ["int8", "int16", "int32"]:
            code.append(f"{indent}{{")
            code.append(f"{indent}    int val;")
            code.append(f"{indent}    err = cbor_value_get_int({value_name}, &val);")
            code.append(
                f"{indent}    {var_name} = ({ros_type_to_c_type(type_name)})val;"
            )
            code.append(f"{indent}}}")
        elif type_name == "int64":
            code.append(f"{indent}{{")
            code.append(f"{indent}    int64_t val;")
            code.append(f"{indent}    err = cbor_value_get_int64({value_name}, &val);")
            code.append(f"{indent}    {var_name} = val;")
            code.append(f"{indent}}}")
        elif type_name in ["uint8", "uint16", "uint32"]:
            code.append(f"{indent}{{")
            code.append(f"{indent}    uint64_t val;")
            code.append(f"{indent}    err = cbor_value_get_uint64({value_name}, &val);")
            code.append(
                f"{indent}    {var_name} = ({ros_type_to_c_type(type_name)})val;"
            )
            code.append(f"{indent}}}")
        elif type_name == "uint64":
            code.append(
                f"{indent}err = cbor_value_get_uint64({value_name}, &{var_name});"
            )
        elif type_name in ["float32", "float64"]:
            code.append(f"{indent}{{")
            code.append(f"{indent}    double val;")
            code.append(f"{indent}    err = cbor_value_get_double({value_name}, &val);")
            code.append(
                f"{indent}    {var_name} = ({ros_type_to_c_type(type_name)})val;"
            )
            code.append(f"{indent}}}")
    else:
        # Nested message
        code.append(
            f"{indent}err = {field.type_name}_decode({value_name}, &{var_name});"
        )

    code.append(f"{indent}if (err != CborNoError) return err;")
    code.append(f"{indent}err = cbor_value_advance({value_name});")
    code.append(f"{indent}if (err != CborNoError) return err;")
    return "\n".join(code)


def generate_implementation(message: Message, output_dir: str) -> str:
    """Generate C17 implementation file"""
    impl_name = f"{message.name}_msg.c"
    impl_path = os.path.join(output_dir, impl_name)

    with open(impl_path, "w") as f:
        # Includes
        f.write(f'#include "{message.name}_msg.h"\n')
        f.write("#include <string.h>\n\n")

        # Encode function
        f.write(
            f"CborError {message.name}_encode(CborEncoder *encoder, const {message.name}_t *msg) {{\n"
        )
        f.write("    CborError err;\n")
        f.write("    CborEncoder map_encoder;\n\n")

        # Create map with number of fields
        num_fields = len(message.fields)
        f.write(
            f"    err = cbor_encoder_create_map(encoder, &map_encoder, {num_fields});\n"
        )
        f.write("    if (err != CborNoError) return err;\n\n")

        # Encode each field as key-value pair
        for field in message.fields:
            f.write(f"    // Encode {field.field_name}\n")
            f.write(
                f'    err = cbor_encode_text_stringz(&map_encoder, "{field.field_name}");\n'
            )
            f.write("    if (err != CborNoError) return err;\n")
            f.write(generate_encode_field(field, "&map_encoder", indent="    "))
            f.write("\n\n")

        # Close map
        f.write("    err = cbor_encoder_close_container(encoder, &map_encoder);\n")
        f.write("    return err;\n")
        f.write("}\n\n")

        # Decode function
        f.write(
            f"CborError {message.name}_decode(CborValue *value, {message.name}_t *msg) {{\n"
        )
        f.write("    CborError err;\n")
        f.write("    CborValue map;\n\n")

        f.write("    if (!cbor_value_is_map(value)) {\n")
        f.write("        return CborErrorIllegalType;\n")
        f.write("    }\n\n")

        f.write("    err = cbor_value_enter_container(value, &map);\n")
        f.write("    if (err != CborNoError) return err;\n\n")

        # Decode each field
        f.write("    while (!cbor_value_at_end(&map)) {\n")
        f.write("        // Read key\n")
        f.write("        if (!cbor_value_is_text_string(&map)) {\n")
        f.write("            return CborErrorIllegalType;\n")
        f.write("        }\n\n")

        f.write("        char key[64];\n")
        f.write("        size_t key_len = sizeof(key);\n")
        f.write(
            "        err = cbor_value_copy_text_string(&map, key, &key_len, NULL);\n"
        )
        f.write("        if (err != CborNoError) return err;\n\n")

        f.write("        err = cbor_value_advance(&map);\n")
        f.write("        if (err != CborNoError) return err;\n\n")

        # Generate if-else chain for field matching
        for i, field in enumerate(message.fields):
            if_keyword = "if" if i == 0 else "} else if"
            f.write(
                f'        {if_keyword} (strcmp(key, "{field.field_name}") == 0) {{\n'
            )
            f.write(generate_decode_field(field, "&map", indent="            "))
            f.write("\n")

        f.write("        } else {\n")
        f.write("            // Unknown field, skip it\n")
        f.write("            err = cbor_value_advance(&map);\n")
        f.write("            if (err != CborNoError) return err;\n")
        f.write("        }\n")
        f.write("    }\n\n")

        f.write("    err = cbor_value_leave_container(value, &map);\n")
        f.write("    return err;\n")
        f.write("}\n")

    return impl_path


def main():
    parser = argparse.ArgumentParser(
        description="Generate C17 CBOR encoding/decoding from ROS .msg files"
    )
    parser.add_argument("input_file", help="Path to input ROS .msg file")
    parser.add_argument(
        "output_dir", help="Output directory for generated .c and .h files"
    )

    args = parser.parse_args()

    # Validate input file
    if not os.path.isfile(args.input_file):
        print(f"Error: Input file '{args.input_file}' does not exist", file=sys.stderr)
        sys.exit(1)

    if not args.input_file.endswith(".msg"):
        print(f"Error: Input file must have .msg extension", file=sys.stderr)
        sys.exit(1)

    # Validate/create output directory
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    if not os.path.isdir(args.output_dir):
        print(
            f"Error: Output path '{args.output_dir}' is not a directory",
            file=sys.stderr,
        )
        sys.exit(1)

    print(f"Input file: {args.input_file}")
    print(f"Output directory: {args.output_dir}")
    print()

    # Parse message file
    message = parse_msg_file(args.input_file)
    print(f"Parsed message: {message.name}")
    print(f"  Fields: {len(message.fields)}")
    print(f"  Imports: {len(message.imports)}")
    print()

    # Generate header file
    header_path = generate_header(message, args.output_dir)
    print(f"Generated: {header_path}")

    # Generate implementation file
    impl_path = generate_implementation(message, args.output_dir)
    print(f"Generated: {impl_path}")
    print()
    print("Done!")


if __name__ == "__main__":
    main()
