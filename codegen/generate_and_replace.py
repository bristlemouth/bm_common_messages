#!/usr/bin/env python3
"""
Script to generate CBOR message files and optionally replace existing handwritten files.

This script provides safe, manual control over code generation and replacement.
It will never overwrite files without explicit user confirmation.
"""

import os
import sys
import shutil
import argparse
from pathlib import Path
from msg_parser import MsgParser
from c_generator import CGenerator


def main():
    parser = argparse.ArgumentParser(
        description="Generate CBOR message files from .msg definitions"
    )
    parser.add_argument(
        "--preview",
        action="store_true",
        help="Generate files to codegen/preview directory (default behavior)"
    )
    parser.add_argument(
        "--replace",
        action="store_true",
        help="Replace existing source files (DANGEROUS - requires confirmation)"
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="Skip confirmation prompts (use with --replace)"
    )
    parser.add_argument(
        "--files",
        nargs="*",
        help="Specific files to replace (default: all generated files)"
    )

    args = parser.parse_args()

    # Set up paths
    script_dir = Path(__file__).parent
    msg_dir = script_dir.parent / "msg"

    if args.replace:
        output_dir = script_dir.parent  # Replace files in main directory
        if not args.force:
            print("WARNING: This will overwrite existing source files!")
            print("Generated files will replace handwritten implementations.")
            print("Make sure you have committed your changes to git first.")
            response = input("Continue? (yes/NO): ")
            if response.lower() != 'yes':
                print("Aborted.")
                sys.exit(0)
    else:
        output_dir = script_dir / "preview"  # Safe preview directory
        output_dir.mkdir(exist_ok=True)

    # Run the parser and generator
    try:
        msg_parser = MsgParser(str(msg_dir))
        messages = msg_parser.parse_all()

        generator = CGenerator(msg_parser, str(output_dir))
        generator.generate_all()

        print(f"\nSuccessfully generated {len(messages)} message files in {output_dir}")

        if args.replace:
            # List what was replaced
            print("\nReplaced files:")
            for name in messages.keys():
                print(f"  - {name}_msg.c")
                print(f"  - {name}_msg.h")
        else:
            print(f"\nTo replace existing files, run:")
            print(f"  python3 {__file__} --replace")
            print(f"  (This will require confirmation unless --force is used)")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == '__main__':
    main()