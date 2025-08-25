# CMake file for code generation tests
# This builds tests that compare handwritten vs generated implementations

cmake_minimum_required(VERSION 3.16)

# Only build if we're in the codegen directory explicitly
if(NOT CMAKE_CURRENT_SOURCE_DIR MATCHES "codegen$")
    return()
endif()

# Find required dependencies
find_package(Python3 COMPONENTS Interpreter)
if(NOT Python3_FOUND)
    message(STATUS "Python3 not found - codegen tests disabled")
    return()
endif()

# Check for GoogleTest
find_package(GTest)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Set up paths
set(MSG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../msg)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(TINYCBOR_DIR ${SRC_DIR}/third_party/tinycbor/src)

# Test target for generated message comparison
add_executable(test_generated_messages
    test_generated_messages.cpp
    ${SRC_DIR}/bm_messages_helper.c
    ${SRC_DIR}/sensor_header_msg.c
    ${TINYCBOR_DIR}/cborencoder.c
    ${TINYCBOR_DIR}/cborparser.c
)

target_include_directories(test_generated_messages PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SRC_DIR}
    ${TINYCBOR_DIR}
)

target_link_libraries(test_generated_messages
    gtest
    gtest_main
)

# Add custom target for running Python-based comparison tests
add_custom_target(run_comparison_tests
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/build_and_test.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running handwritten vs generated CBOR comparison tests"
    VERBATIM
)

# Make it depend on message generation (if available)
if(TARGET generate_cbor_messages_preview)
    add_dependencies(run_comparison_tests generate_cbor_messages_preview)
endif()