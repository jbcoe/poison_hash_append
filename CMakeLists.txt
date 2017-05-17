cmake_minimum_required(VERSION 2.8)

project(hash_append)

add_compile_options("-std=c++14")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(test_hash_append test_hash_append.cpp)

enable_testing()
add_test(
  NAME test_hash_append
  COMMAND test_hash_append
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

