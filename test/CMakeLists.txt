include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# - Create Unit Test
#
# This function creates a unit test with required source files.
# Coverage report information is compiled in for the file to be tested.
#
# test_name - the name to call the test
# srcs - list of sources necessary to build the test including the file to be tested

function(create_gtest test_name  srcs)
    set(GTEST_NAME ${test_name})
    file(GLOB TEST_STUBS stubs/*.c)
    list(FILTER TEST_STUBS EXCLUDE REGEX "${test_name}")
    add_executable(${GTEST_NAME})
    target_sources(${GTEST_NAME}
        PRIVATE
        ${srcs}
        ${test_name}_ut.cpp
    )
    gtest_discover_tests(${GTEST_NAME})
endfunction()

#
# bm_common_messages tests
#
include_directories(bm_common_messages_tests
    ${SRC_DIR}
    ${SRC_DIR}/third_party/tinycbor/src
    ${TEST_DIR}
)

link_libraries(
    gmock
    gtest_main
)

set(BM_COMMON_MESSAGES_SRCS
    # Unit test wrapper for test
    bm_common_messages_tests_ut.cpp

    # msg files for testing
    ${SRC_DIR}/bm_messages_helper.c
    ${SRC_DIR}/barometric_pressure_data_msg.cpp
    ${SRC_DIR}/bm_soft_data_msg.cpp
    ${SRC_DIR}/bm_rbr_data_msg.cpp
    ${SRC_DIR}/bm_seapoint_turbidity_data_msg.cpp
    ${SRC_DIR}/sensor_header_msg.cpp
    ${SRC_DIR}/sensor_header_msg.c
    ${SRC_DIR}/device_test_svc_reply_msg.cpp
    ${SRC_DIR}/device_test_svc_request_msg.cpp
    ${SRC_DIR}/bm_rbr_pressure_difference_signal_msg.cpp
    ${SRC_DIR}/pme_dissolved_oxygen_msg.cpp
    ${SRC_DIR}/pme_wipe_msg.cpp
    ${SRC_DIR}/power_reading_msg.cpp
    ${SRC_DIR}/power_reading_averages_msg.cpp
    ${SRC_DIR}/aanderaa_conductivity_msg.cpp
    ${SRC_DIR}/aanderaa_current_meter_msg.cpp

    # support files
    ${SRC_DIR}/third_party/tinycbor/src/cborparser.c
    ${SRC_DIR}/third_party/tinycbor/src/cborencoder.c
)

create_gtest("bm_common_messages_tests" "${BM_COMMON_MESSAGES_SRCS}")

set(BM_BOREALIS_SRCS
    # Unit test wrapper for test
    bm_borealis_ut.cpp

    # msg files for testing
    ${SRC_DIR}/bm_messages_helper.c
    ${SRC_DIR}/bm_borealis.cpp
    ${SRC_DIR}/sensor_header_msg.cpp
    ${SRC_DIR}/sensor_header_msg.c

    # support files
    ${SRC_DIR}/third_party/tinycbor/src/cborparser.c
    ${SRC_DIR}/third_party/tinycbor/src/cborencoder.c
)

create_gtest("bm_borealis" "${BM_BOREALIS_SRCS}")

set(POWER_INFO_SRCS
    # Unit test wrapper for test
    power_info_ut.cpp

    # msg files for testing
    ${SRC_DIR}/bm_messages_helper.c
    ${SRC_DIR}/power_info_reply_msg.c

    # support files
    ${SRC_DIR}/third_party/tinycbor/src/cborparser.c
    ${SRC_DIR}/third_party/tinycbor/src/cborencoder.c
)

create_gtest("power_info" "${POWER_INFO_SRCS}")
