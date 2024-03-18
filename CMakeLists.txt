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
add_subdirectory("third_party/googletest")
add_subdirectory("test")

else()
#
# if we are a subproject
# Use target_link_libraries(${EXECUTABLE} bm_common_messages) in parent CMakeLists.txt to use
# Set target compiler flags with the BM_COMMON_MESSAGES_COMPILER_FLAGS variable
#
set(BM_COMMON_MESSAGES_DIR ${CMAKE_CURRENT_LIST_DIR})
message(status "BM COMMON MESSAGES DIR ${BM_COMMON_MESSAGES_DIR}")

set(BM_COMMON_MESSAGES_INCLUDES
    ${BM_COMMON_MESSAGES_DIR}
    )

set(BM_COMMON_MESSAGES_INCLUDES ${BM_COMMON_MESSAGES_INCLUDES} PARENT_SCOPE)

endif()