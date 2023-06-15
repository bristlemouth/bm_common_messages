cmake_minimum_required(VERSION 3.18)
project(bm_common_messages
	LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
#
# if we are a subproject, export as a library
# Use target_link_libraries(${EXECUTABLE} bm_common_messages) in parent CMakeLists.txt to use
# Set target compiler flags with the BM_COMMON_MESSAGES_COMPILER_FLAGS variable
#
set(BM_COMMON_MESSAGES_DIR ${CMAKE_CURRENT_LIST_DIR})
message(status "BM COMMON MESSAGES DIR ${BM_COMMON_MESSAGES_DIR}")

set(BM_COMMON_MESSAGES_INCLUDES
    ${BM_COMMON_MESSAGES_DIR}
    )

add_library(bm_common_messages INTERFACE)
target_compile_options(bm_common_messages PRIVATE ${BM_COMMON_MESSAGES_COMPILER_FLAGS})
target_include_directories(bm_common_messages PRIVATE ${BM_COMMON_MESSAGES_INCLUDES})

set(BM_COMMON_MESSAGES_INCLUDES ${BM_COMMON_MESSAGES_INCLUDES} PARENT_SCOPE)

endif()