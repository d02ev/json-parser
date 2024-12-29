cmake_minimum_required(VERSION 3.30)
project(json_parser VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/includes)
add_subdirectory(src)