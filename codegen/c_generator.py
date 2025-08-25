#!/usr/bin/env python3
"""
C code generator for CBOR message encoding/decoding.

Generates C17-compatible code for encoding and decoding messages to/from CBOR format,
following the patterns established in the existing bm_common_messages codebase.
"""

from typing import List, Dict, Optional, Set
from pathlib import Path
import textwrap
from msg_parser import MsgParser, Message, Field


class CGenerator:
    """Generates C code for CBOR message encoding/decoding."""

    def __init__(self, parser: MsgParser, output_dir: str = "."):
        self.parser = parser
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)

    def generate_all(self) -> None:
        """Generate code for all parsed messages in dependency order."""
        messages = self.parser.parsed_messages
        dependency_order = self.parser.resolve_dependencies()

        for msg_name in dependency_order:
            if msg_name in messages:
                message = messages[msg_name]
                self.generate_message(message)

    def generate_message(self, message: Message) -> None:
        """Generate header and implementation files for a message."""
        header_content = self._generate_header(message)
        impl_content = self._generate_implementation(message)

        header_path = self.output_dir / f"{message.name}_msg.h"
        impl_path = self.output_dir / f"{message.name}_msg.c"

        with open(header_path, 'w') as f:
            f.write(header_content)

        with open(impl_path, 'w') as f:
            f.write(impl_content)

        print(f"Generated {header_path} and {impl_path}")

    def _generate_header(self, message: Message) -> str:
        """Generate header file content."""
        header_guard = f"{message.name.upper()}_MSG_H"

        # Generate struct definition
        struct_fields = []
        for field in message.fields:
            c_type = self.parser.get_c_type(field, message.imports)
            struct_fields.append(f"  {c_type} {field.field_name};")

        struct_def = "{\n" + "\n".join(struct_fields) + "\n}"
        field_count = len(message.fields)

        # Generate function declarations
        encode_params = []
        decode_params = []

        for field in message.fields:
            c_type = self.parser.get_c_type(field, message.imports)
            encode_params.append(f"const {c_type} {field.field_name}")

            # For decode, we need pointers for output parameters
            if c_type == "char*":
                decode_params.append(f"char ** const {field.field_name}_p")
            else:
                decode_params.append(f"{c_type} * const {field.field_name}_p")

        if encode_params:
            params_formatted = []
            for param in encode_params:
                params_formatted.append(" " * 31 + param)
            encode_sig = f"CborError {message.name}_encode(CborEncoder * const map_encoder,\n" + \
                        ",\n".join(params_formatted)
        else:
            encode_sig = f"CborError {message.name}_encode(CborEncoder * const map_encoder"

        # Format decode parameters with consistent indentation
        formatted_decode_params = []
        for param in decode_params:
            formatted_decode_params.append(" " * 31 + param)
        formatted_decode_params.append(" " * 31 + "CborValue * const map")
        decode_sig = f"CborError {message.name}_decode(" + ",\n".join(formatted_decode_params)

        # Build includes
        includes = ['#include <stdint.h>', '#include "cbor.h"']
        for import_name in message.imports:
            includes.append(f'#include "{import_name}_msg.h"')

        header_content = f"""#pragma once
{chr(10).join(includes)}

#ifdef __cplusplus
namespace {self._to_camel_case(message.name)}Msg {{

constexpr size_t NUM_FIELDS = {field_count};

struct Data {struct_def};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

}} // namespace {self._to_camel_case(message.name)}Msg

extern "C" {{
#endif

{encode_sig});

{decode_sig});

#ifdef __cplusplus
}}
#endif
"""
        return header_content

    def _generate_implementation(self, message: Message) -> str:
        """Generate implementation file content."""
        # Generate encode function
        encode_body = self._generate_encode_body(message)
        decode_body = self._generate_decode_body(message)

        # Generate function signatures
        encode_params = []
        decode_params = []

        for field in message.fields:
            c_type = self.parser.get_c_type(field, message.imports)
            encode_params.append(f"const {c_type} {field.field_name}")

            if c_type == "char*":
                decode_params.append(f"char ** const {field.field_name}_p")
            else:
                decode_params.append(f"{c_type} * const {field.field_name}_p")

        if encode_params:
            params_formatted = []
            for param in encode_params:
                params_formatted.append(" " * 31 + param)
            encode_sig = f"CborError {message.name}_encode(CborEncoder * const map_encoder,\n" + \
                        ",\n".join(params_formatted)
        else:
            encode_sig = f"CborError {message.name}_encode(CborEncoder * const map_encoder"

        # Format decode parameters with consistent indentation
        formatted_decode_params = []
        for param in decode_params:
            formatted_decode_params.append(" " * 31 + param)
        formatted_decode_params.append(" " * 31 + "CborValue * const map")
        decode_sig = f"CborError {message.name}_decode(" + ",\n".join(formatted_decode_params)

        impl_content = f"""#include "{message.name}_msg.h"
#include "bm_config.h"

{encode_sig}) {{
{encode_body}
}}

{decode_sig}) {{
{decode_body}
}}
"""
        return impl_content

    def _generate_encode_body(self, message: Message) -> str:
        """Generate the body of the encode function."""
        lines = []
        lines.append("    CborError err = CborNoError;")
        lines.append("")
        lines.append("    do {")

        for field in message.fields:
            c_type = self.parser.get_c_type(field, message.imports)
            field_name = field.field_name.replace("optional_", "")  # Remove optional prefix for CBOR key

            # Generate key encoding
            lines.append(f'        // {field_name}')
            lines.append(f'        err = cbor_encode_text_stringz(map_encoder, "{field_name}");')
            lines.append(f'        if (err != CborNoError) {{')
            lines.append(f'            bm_debug("cbor_encode_text_stringz failed for {field_name} key: %d\\n", err);')
            lines.append(f'            if (err != CborErrorOutOfMemory) {{')
            lines.append(f'                break;')
            lines.append(f'            }}')
            lines.append(f'        }}')

            # Generate value encoding based on type
            if c_type in ['uint8_t', 'uint16_t', 'uint32_t', 'uint64_t']:
                lines.append(f'        err = cbor_encode_uint(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_uint failed for {field_name} value: %d\\n'
            elif c_type in ['int8_t', 'int16_t', 'int32_t', 'int64_t']:
                lines.append(f'        err = cbor_encode_int(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_int failed for {field_name} value: %d\\n'
            elif c_type == 'float':
                lines.append(f'        err = cbor_encode_float(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_float failed for {field_name} value: %d\\n'
            elif c_type == 'double':
                lines.append(f'        err = cbor_encode_double(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_double failed for {field_name} value: %d\\n'
            elif c_type == 'bool':
                lines.append(f'        err = cbor_encode_boolean(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_boolean failed for {field_name} value: %d\\n'
            elif c_type == 'char*':
                # String fields need length parameter - for now assume null-terminated
                lines.append(f'        err = cbor_encode_text_stringz(map_encoder, {field.field_name});')
                error_msg = f'cbor_encode_text_stringz failed for {field_name} value: %d\\n'
            elif c_type == 'uint8_t*':  # Array type - needs length
                # For array fields, we need the corresponding length field
                length_field = self._find_length_field(field.field_name, message.fields)
                if length_field:
                    lines.append(f'        err = cbor_encode_byte_string(map_encoder, {field.field_name}, {length_field});')
                    error_msg = f'cbor_encode_byte_string failed for {field_name} value: %d\\n'
                else:
                    lines.append(f'        // TODO: Array field {field.field_name} needs corresponding length field')
                    lines.append(f'        err = CborErrorUnknownType;')
                    error_msg = f'missing length field for array {field_name}: %d\\n'
            elif c_type.endswith('_t'):  # Imported struct type
                # For imported types, we need to call their encode function
                import_name = c_type[:-2]  # Remove _t suffix
                lines.append(f'        err = {import_name}_encode(map_encoder, {field.field_name});')
                error_msg = f'{import_name}_encode failed for {field_name}: %d\\n'
            else:
                lines.append(f'        // TODO: Handle type {c_type}')
                lines.append(f'        err = CborErrorUnknownType;')
                error_msg = f'unknown type {c_type} for {field_name}: %d\\n'

            lines.append(f'        if (err != CborNoError) {{')
            lines.append(f'            bm_debug("{error_msg}", err);')
            lines.append(f'            if (err != CborErrorOutOfMemory) {{')
            lines.append(f'                break;')
            lines.append(f'            }}')
            lines.append(f'        }}')
            lines.append("")

        lines.append("    } while (0);")
        lines.append("")
        lines.append("    return err;")

        return "\n".join(lines)

    def _generate_decode_body(self, message: Message) -> str:
        """Generate the body of the decode function."""
        lines = []
        lines.append("    CborError err = CborNoError;")
        lines.append("")

        # Determine which temporary variables we need
        needs_uint64 = any(self.parser.get_c_type(field, message.imports) in
                          ['uint8_t', 'uint16_t', 'uint32_t', 'uint64_t'] for field in message.fields)
        needs_int64 = any(self.parser.get_c_type(field, message.imports) in
                         ['int8_t', 'int16_t', 'int32_t', 'int64_t'] for field in message.fields)
        needs_float = any(self.parser.get_c_type(field, message.imports) == 'float' for field in message.fields)
        needs_double = any(self.parser.get_c_type(field, message.imports) == 'double' for field in message.fields)
        needs_bool = any(self.parser.get_c_type(field, message.imports) == 'bool' for field in message.fields)

        # Declare needed temporary variables
        if needs_uint64:
            lines.append("    uint64_t tmp_uint64;")
        if needs_int64:
            lines.append("    int64_t tmp_int64;")
        if needs_float:
            lines.append("    float tmp_float;")
        if needs_double:
            lines.append("    double tmp_double;")
        if needs_bool:
            lines.append("    bool tmp_bool;")

        lines.append("")
        lines.append("    do {")

        for field in message.fields:
            c_type = self.parser.get_c_type(field, message.imports)
            field_name = field.field_name

            # Handle optional fields - they might need special encoding
            if field.is_optional:
                lines.append(f'        // {field_name} (optional)')
                lines.append(f'        // Note: Optional fields are encoded unconditionally in this implementation')
            else:
                lines.append(f'        // {field_name}')
            lines.append(f'        if (!cbor_value_is_text_string(map)) {{')
            lines.append(f'            err = CborErrorIllegalType;')
            lines.append(f'            bm_debug("expected string key but got something else\\n");')
            lines.append(f'            break;')
            lines.append(f'        }}')
            lines.append(f'        err = cbor_value_advance(map);')
            lines.append(f'        if (err != CborNoError) {{')
            lines.append(f'            break;')
            lines.append(f'        }}')

            # Generate value decoding based on type
            if c_type in ['uint8_t', 'uint16_t', 'uint32_t', 'uint64_t']:
                lines.append(f'        err = cbor_value_get_uint64(map, &tmp_uint64);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        *{field.field_name}_p = tmp_uint64;')
            elif c_type in ['int8_t', 'int16_t', 'int32_t', 'int64_t']:
                lines.append(f'        err = cbor_value_get_int64(map, &tmp_int64);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        *{field.field_name}_p = tmp_int64;')
            elif c_type == 'float':
                lines.append(f'        err = cbor_value_get_float(map, &tmp_float);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        *{field.field_name}_p = tmp_float;')
            elif c_type == 'double':
                lines.append(f'        err = cbor_value_get_double(map, &tmp_double);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        *{field.field_name}_p = tmp_double;')
            elif c_type == 'bool':
                lines.append(f'        err = cbor_value_get_boolean(map, &tmp_bool);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        *{field.field_name}_p = tmp_bool;')
            elif c_type == 'char*':
                # String decoding - need to handle memory allocation
                lines.append(f'        size_t tmp_string_len;')
                lines.append(f'        err = cbor_value_calculate_string_length(map, &tmp_string_len);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        // Note: Caller must allocate memory for string')
                lines.append(f'        // This is a limitation of the low-level C API')
                lines.append(f'        err = CborErrorUnknownType; // TODO: Implement string memory allocation')
            elif c_type == 'uint8_t*':  # Array type
                # Array decoding - similar memory management issues
                lines.append(f'        size_t tmp_array_len;')
                lines.append(f'        err = cbor_value_calculate_string_length(map, &tmp_array_len);')
                lines.append(f'        if (err != CborNoError) {{')
                lines.append(f'            break;')
                lines.append(f'        }}')
                lines.append(f'        // Note: Caller must allocate memory for array data')
                lines.append(f'        // This is a limitation of the low-level C API')
                lines.append(f'        err = CborErrorUnknownType; // TODO: Implement array memory allocation')
            elif c_type.endswith('_t'):  # Imported struct type
                import_name = c_type[:-2]  # Remove _t suffix
                lines.append(f'        err = {import_name}_decode({field.field_name}_p, map);')
            else:
                lines.append(f'        // TODO: Handle type {c_type}')
                lines.append(f'        err = CborErrorUnknownType;')

            lines.append(f'        err = cbor_value_advance(map);')
            lines.append(f'        if (err != CborNoError) {{')
            lines.append(f'            break;')
            lines.append(f'        }}')
            lines.append("")

        lines.append("    } while (0);")
        lines.append("")
        lines.append("    return err;")

        return "\n".join(lines)

    def _find_length_field(self, array_field_name: str, fields: List[Field]) -> Optional[str]:
        """Find the corresponding length field for an array field."""
        # Common patterns: cbor_data -> cbor_encoded_map_len, data -> data_len
        possible_length_names = [
            f"{array_field_name}_len",
            f"{array_field_name}_length",
            f"{array_field_name}_size",
        ]

        # Special cases
        if array_field_name == "cbor_data":
            possible_length_names.append("cbor_encoded_map_len")

        for field in fields:
            if field.field_name in possible_length_names:
                return field.field_name
        return None

    def _to_camel_case(self, snake_str: str) -> str:
        """Convert snake_case to CamelCase."""
        components = snake_str.split('_')
        return ''.join(word.capitalize() for word in components)


if __name__ == '__main__':
    import sys

    if len(sys.argv) != 2:
        print("Usage: python c_generator.py <msg_directory>")
        sys.exit(1)

    msg_dir = sys.argv[1]
    parser = MsgParser(msg_dir)

    try:
        messages = parser.parse_all()
        generator = CGenerator(parser)
        generator.generate_all()
        print(f"\nGenerated code for {len(messages)} messages in {generator.output_dir}")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)