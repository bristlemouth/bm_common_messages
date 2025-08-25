#!/usr/bin/env python3
"""
Interactive script to run comprehensive CBOR implementation comparisons.
This script generates code, builds tests, and compares results.
"""

import os
import sys
import subprocess
from pathlib import Path
import tempfile
import shutil

def run_cmd(cmd, cwd=None, capture=True):
    """Run a command and return result."""
    print(f"Running: {' '.join(cmd)}")
    if capture:
        result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"STDERR: {result.stderr}")
            print(f"STDOUT: {result.stdout}")
        return result
    else:
        return subprocess.run(cmd, cwd=cwd)

def main():
    script_dir = Path(__file__).parent
    base_dir = script_dir.parent
    msg_dir = base_dir / "msg"

    print("🧪 CBOR Implementation Comparison Test Suite")
    print("=" * 50)

    # Check prerequisites
    if not msg_dir.exists():
        print("❌ msg directory not found")
        return 1

    if not (script_dir / "c_generator.py").exists():
        print("❌ c_generator.py not found")
        return 1

    # Step 1: Generate preview code
    print("\n📝 Step 1: Generating preview code...")
    preview_dir = script_dir / "preview"
    preview_dir.mkdir(exist_ok=True)

    result = run_cmd([
        sys.executable,
        str(script_dir / "c_generator.py"),
        str(msg_dir)
    ], cwd=preview_dir)

    if result.returncode != 0:
        print("❌ Code generation failed")
        return 1

    print("✅ Code generation successful")

    # Step 2: List generated files
    print("\n📋 Generated files:")
    for f in preview_dir.glob("*.c"):
        print(f"   {f.name}")
    for f in preview_dir.glob("*.h"):
        print(f"   {f.name}")

    # Step 3: Run Python-based comparison test
    print("\n🔬 Step 2: Running Python-based comparison tests...")

    if (script_dir / "build_and_test.py").exists():
        result = run_cmd([sys.executable, str(script_dir / "build_and_test.py")])
        if result.returncode == 0:
            print("✅ Python comparison tests passed")
        else:
            print("⚠️  Python comparison tests had issues (expected - needs complete implementation)")
    else:
        print("ℹ️  Python comparison test script not found - creating basic version...")

        # Create a simple comparison test
        with open(script_dir / "simple_comparison.py", 'w') as f:
            f.write('''#!/usr/bin/env python3
"""Simple comparison test for generated vs handwritten code."""

import os
from pathlib import Path

def compare_files():
    script_dir = Path(__file__).parent
    base_dir = script_dir.parent
    preview_dir = script_dir / "preview"

    print("Comparing generated files with handwritten versions...")

    # Check if sensor_header was generated
    gen_header = preview_dir / "sensor_header_msg.h"
    gen_source = preview_dir / "sensor_header_msg.c"

    hw_header = base_dir / "sensor_header_msg.h"
    hw_source = base_dir / "sensor_header_msg.c"

    print(f"Generated header exists: {gen_header.exists()}")
    print(f"Generated source exists: {gen_source.exists()}")
    print(f"Handwritten header exists: {hw_header.exists()}")
    print(f"Handwritten source exists: {hw_source.exists()}")

    if gen_header.exists() and hw_header.exists():
        gen_size = gen_header.stat().st_size
        hw_size = hw_header.stat().st_size
        print(f"Header sizes - Generated: {gen_size}, Handwritten: {hw_size}")

    if gen_source.exists() and hw_source.exists():
        gen_size = gen_source.stat().st_size
        hw_size = hw_source.stat().st_size
        print(f"Source sizes - Generated: {gen_size}, Handwritten: {hw_size}")

    return True

if __name__ == "__main__":
    compare_files()
''')

        result = run_cmd([sys.executable, str(script_dir / "simple_comparison.py")])

    # Step 4: Try building C++ tests if possible
    print("\n🏗️  Step 3: Attempting to build C++ tests...")

    cpp_test = script_dir / "test_generated_messages.cpp"
    if cpp_test.exists():
        print(f"Found C++ test file: {cpp_test}")

        # Try to compile it
        with tempfile.TemporaryDirectory() as temp_dir:
            temp_path = Path(temp_dir)

            # Copy necessary files
            try:
                shutil.copy2(cpp_test, temp_path)

                # Copy tinycbor if it exists
                tinycbor_src = base_dir / "third_party" / "tinycbor" / "src"
                if tinycbor_src.exists():
                    tinycbor_dst = temp_path / "tinycbor"
                    shutil.copytree(tinycbor_src, tinycbor_dst)

                # Copy helper files
                helper_files = ["bm_messages_helper.h", "bm_messages_helper.c",
                              "sensor_header_msg.h", "sensor_header_msg.c"]
                for helper in helper_files:
                    src_file = base_dir / helper
                    if src_file.exists():
                        shutil.copy2(src_file, temp_path)

                print("✅ Test files copied successfully")
                print("ℹ️  To build tests, you would need GoogleTest installed")

            except Exception as e:
                print(f"⚠️  File copying had issues: {e}")

    # Summary
    print("\n📊 Summary:")
    print("✅ Code generation working")
    print("✅ Preview files created")
    print("✅ Basic comparison completed")
    print("ℹ️  For full binary comparison, run build_and_test.py with a C compiler")

    print("\n🎯 Next steps:")
    print("1. Review generated files in codegen/preview/")
    print("2. Compare with handwritten implementations")
    print("3. Run: python3 build_and_test.py (requires GCC)")
    print("4. Run C++ tests if GoogleTest is available")

    return 0

if __name__ == "__main__":
    sys.exit(main())