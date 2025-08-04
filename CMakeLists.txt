cmake_minimum_required(VERSION 3.18)
project(bm_common_messages
	LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# If we're not a subproject, build the tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_compile_options(
    -Wall
    -Wextra
    -Werror
    -O0
    -DCI_TEST
    -g
    )
message(STATUS "Native compiling - Building Tests")
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

# Setup testing
include(CTest)
add_subdirectory("test")
include_directories("test")

else()
#
# if we are a subproject
# Use target_link_libraries(${EXECUTABLE} bm_common_messages) in parent CMakeLists.txt to use
# Set target compiler flags with the BM_COMMON_MESSAGES_COMPILER_FLAGS variable
#
set(BM_COMMON_MESSAGES_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SOURCES
    aanderaa_current_meter_msg.cpp
    barometric_pressure_data_msg.cpp
    bm_borealis.cpp
    bm_messages_helper.c
    bm_rbr_data_msg.cpp
    bm_rbr_pressure_difference_signal_msg.cpp
    bm_seapoint_turbidity_data_msg.cpp
    bm_soft_data_msg.cpp
        aanderaa_conductivity_msg.cpp
    config_cbor_map_srv_reply_msg.c
    config_cbor_map_srv_request_msg.c
    device_test_svc_reply_msg.cpp
    device_test_svc_request_msg.cpp
    pme_dissolved_oxygen_msg.cpp
    pme_wipe_msg.cpp
    power_info_reply_msg.c
    sensor_header_msg.cpp
    sensor_header_msg.c
    sys_info_svc_reply_msg.c
)

add_library(bmmessages ${SOURCES})

target_include_directories(bmmessages PUBLIC
    .
)

target_link_libraries(bmmessages PRIVATE bmcommon)

endif()
