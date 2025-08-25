# CMake utilities for CBOR message code generation
# This file provides optional code generation - never runs automatically

# Optional target to generate C files from .msg files
# Usage: cmake --build . --target generate_cbor_messages_preview
function(add_cbor_message_generation_target)
    # Set up paths
    set(MSG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/msg)
    set(CODEGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/codegen)
    set(PREVIEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/codegen/preview)

    # Find Python interpreter
    find_package(Python3 COMPONENTS Interpreter)

    if(NOT Python3_FOUND)
        message(STATUS "Python3 not found - message generation target disabled")
        return()
    endif()

    # Find all .msg files
    file(GLOB MSG_FILES "${MSG_DIR}/*.msg")

    if(NOT MSG_FILES)
        message(STATUS "No .msg files found - message generation target disabled")
        return()
    endif()

    # Create preview target that generates to codegen/preview directory
    # This never overwrites existing files in the main source tree
    add_custom_target(generate_cbor_messages_preview
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PREVIEW_DIR}
        COMMAND ${CMAKE_COMMAND} -E chdir ${PREVIEW_DIR} ${Python3_EXECUTABLE} ${CODEGEN_DIR}/c_generator.py ${MSG_DIR}
        DEPENDS ${MSG_FILES}
                ${CODEGEN_DIR}/c_generator.py
                ${CODEGEN_DIR}/msg_parser.py
        COMMENT "Generating CBOR message files to ${PREVIEW_DIR} (preview only - does not overwrite existing files)"
        VERBATIM
    )

    message(STATUS "Code generation target added:")
    message(STATUS "  Run: cmake --build . --target generate_cbor_messages_preview")
    message(STATUS "  Output: ${PREVIEW_DIR}/")
    message(STATUS "  This will NOT modify existing source files")
endfunction()