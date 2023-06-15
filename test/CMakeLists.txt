#
# bm_common_messages tests
#
add_executable(bm_common_messages_tests)
target_include_directories(bm_common_messages_tests
    PRIVATE
    ${SRC_DIR}
    ${TEST_DIR}
)

target_sources(bm_common_messages_tests
    PRIVATE

    # Unit test wrapper for test
    bm_common_messages_tests_ut.cpp
)

target_link_libraries(bm_common_messages_tests gtest gmock gtest_main)

add_test(
  NAME
    bm_common_messages_tests
  COMMAND
    bm_common_messages_tests
  )