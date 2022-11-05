cmake_minimum_required(VERSION 3.10)
project(TableManager)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE DEBUG)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

# SRC_LIST
aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/BPlusTree BPTREE_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/TableManager TABLE_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Utility UTILITY_SRC_LIST)

# INCLUDE_DIR
include_directories(${PROJECT_SOURCE_DIR}/include/BPlusTree)
include_directories(${PROJECT_SOURCE_DIR}/include/TableManager)
include_directories(${PROJECT_SOURCE_DIR}/include/Utility)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${BPTREE_SRC_LIST} ${UTILITY_SRC_LIST} ${TABLE_SRC_LIST})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)