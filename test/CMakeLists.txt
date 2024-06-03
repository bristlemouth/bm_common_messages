#
# bm_common_messages tests
#
add_executable(bm_common_messages_tests)
target_include_directories(bm_common_messages_tests
    PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/third_party/tinycbor/src
    ${TEST_DIR}
)

target_sources(bm_common_messages_tests
    PRIVATE

    # Unit test wrapper for test
    bm_common_messages_tests_ut.cpp

    # msg files for testing
    ${SRC_DIR}/bm_soft_data_msg.cpp
    ${SRC_DIR}/bm_rbr_data_msg.cpp
    ${SRC_DIR}/bm_seapoint_turbidity_data_msg.cpp
    ${SRC_DIR}/sensor_header_msg.cpp
    ${SRC_DIR}/device_test_svc_reply_msg.cpp
    ${SRC_DIR}/device_test_svc_request_msg.cpp
    ${SRC_DIR}/bm_rbr_pressure_difference_signal_msg.cpp

    # support files
    ${SRC_DIR}/third_party/tinycbor/src/cborparser.c
    ${SRC_DIR}/third_party/tinycbor/src/cborencoder.c
)

target_link_libraries(bm_common_messages_tests gmock gtest_main)

add_test(
  NAME
    bm_common_messages_tests
  COMMAND
    bm_common_messages_tests
  )
