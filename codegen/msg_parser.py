#!/usr/bin/env python3
"""
ROS-style .msg file parser for CBOR code generation.

Parses message definition files and builds a structured representation
for generating C code with CBOR encoding/decoding functions.
"""

import re
import os
from dataclasses import dataclass
from typing import List, Dict, Optional, Set
from pathlib import Path


@dataclass
class Field:
    """Represents a field in a message definition."""
    type_name: str
    field_name: str
    is_array: bool = False
    is_import: bool = False
    is_optional: bool = False


@dataclass
class Message:
    """Represents a parsed message definition."""
    name: str
    fields: List[Field]
    imports: List[str]


class MsgParser:
    """Parser for ROS-style .msg files."""

    # Type mapping from .msg types to C types
    TYPE_MAPPING = {
        'uint8': 'uint8_t',
        'uint16': 'uint16_t',
        'uint32': 'uint32_t',
        'uint64': 'uint64_t',
        'int8': 'int8_t',
        'int16': 'int16_t',
        'int32': 'int32_t',
        'int64': 'int64_t',
        'float32': 'float',
        'float64': 'double',
        'bool': 'bool',
        'string': 'char*',
        'array': 'uint8_t*',  # Generic array type for now
    }

    def __init__(self, msg_dir: str):
        """Initialize parser with message directory path."""
        self.msg_dir = Path(msg_dir)
        self.parsed_messages: Dict[str, Message] = {}

    def parse_file(self, filename: str) -> Message:
        """Parse a single .msg file."""
        filepath = self.msg_dir / filename

        if not filepath.exists():
            raise FileNotFoundError(f"Message file not found: {filepath}")

        with open(filepath, 'r') as f:
            content = f.read()

        return self._parse_content(content, filename)

    def _parse_content(self, content: str, filename: str) -> Message:
        """Parse message content from string."""
        lines = content.strip().split('\n')

        # Extract message name from filename
        msg_name = Path(filename).stem

        fields = []
        imports = []

        for line_num, line in enumerate(lines, 1):
            line = line.strip()

            # Skip empty lines and comments
            if not line or line.startswith('#'):
                continue

            try:
                field = self._parse_line(line)
                if field:
                    if field.is_import:
                        imports.append(field.type_name)
                    else:
                        fields.append(field)
            except ValueError as e:
                raise ValueError(f"Error parsing {filename}:{line_num}: {e}")

        message = Message(name=msg_name, fields=fields, imports=imports)
        self.parsed_messages[msg_name] = message
        return message

    def _parse_line(self, line: str) -> Optional[Field]:
        """Parse a single line of a message definition."""
        # Handle import statements
        if line.startswith('import '):
            import_name = line[7:].strip()
            return Field(type_name=import_name, field_name='', is_import=True)

        # Parse field definitions: "type field_name" or "optional type field_name" or "enum field_name"
        parts = line.split()
        if len(parts) < 2:
            return None

        # Handle special prefixes
        is_optional = False
        if parts[0] == 'optional':
            is_optional = True
            parts = parts[1:]  # Remove 'optional' prefix
            if len(parts) < 2:
                return None

        type_name = parts[0]
        field_name = parts[1]

        # Handle enum types (enum field_name -> treat as uint32_t)
        if type_name == 'enum':
            type_name = 'uint32'

        # Check if it's an array type
        is_array = type_name == 'array'

        # Handle array bracket notation (future extension)
        if '[' in field_name and ']' in field_name:
            field_name = field_name.split('[')[0]
            is_array = True

        return Field(type_name=type_name, field_name=field_name, is_array=is_array, is_optional=is_optional)

    def get_c_type(self, field: Field, imports: List[str] = None) -> str:
        """Convert message field type to C type."""
        if field.is_import:
            # For imported types, we'll need the corresponding struct
            return f"{field.type_name}_t"

        # Check if this field type is an imported type
        if imports and field.type_name in imports:
            return f"{field.type_name}_t"

        if field.type_name in self.TYPE_MAPPING:
            c_type = self.TYPE_MAPPING[field.type_name]
            return c_type
        else:
            # Check for common typos and provide helpful error messages
            if field.type_name == 'float_64':
                raise ValueError(f"Unknown type '{field.type_name}'. Did you mean 'float64'?")
            # Throw error on unknown types
            raise ValueError(f"Unknown type '{field.type_name}'. Valid types are: {', '.join(self.TYPE_MAPPING.keys())}")

    def parse_all(self) -> Dict[str, Message]:
        """Parse all .msg files in the directory."""
        msg_files = list(self.msg_dir.glob('*.msg'))

        if not msg_files:
            raise ValueError(f"No .msg files found in {self.msg_dir}")

        # Parse all files
        for msg_file in msg_files:
            self.parse_file(msg_file.name)

        return self.parsed_messages

    def resolve_dependencies(self) -> List[str]:
        """Resolve message dependencies and return them in dependency order."""
        # Build dependency graph
        deps = {}
        for name, message in self.parsed_messages.items():
            deps[name] = set(message.imports)

        # Topological sort
        result = []
        visited = set()
        temp_visited = set()

        def visit(name):
            if name in temp_visited:
                raise ValueError(f"Circular dependency detected involving {name}")
            if name in visited:
                return

            temp_visited.add(name)
            for dep in deps.get(name, set()):
                visit(dep)
            temp_visited.remove(name)
            visited.add(name)
            result.append(name)

        for name in deps:
            visit(name)

        return result


if __name__ == '__main__':
    # Test the parser
    import sys

    if len(sys.argv) != 2:
        print("Usage: python msg_parser.py <msg_directory>")
        sys.exit(1)

    msg_dir = sys.argv[1]
    parser = MsgParser(msg_dir)

    try:
        messages = parser.parse_all()
        print(f"Parsed {len(messages)} messages:")

        for name, message in messages.items():
            print(f"\nMessage: {name}")
            if message.imports:
                print(f"  Imports: {', '.join(message.imports)}")
            print(f"  Fields:")
            for field in message.fields:
                try:
                    c_type = parser.get_c_type(field, message.imports)
                    print(f"    {c_type} {field.field_name}")
                except ValueError as e:
                    print(f"    ERROR: {e}")
                    sys.exit(1)

        print(f"\nDependency order: {parser.resolve_dependencies()}")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)