cmake_minimum_required (VERSION 3.10)
project (Crabs)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Crabs _Seams/_Main.cpp)

target_include_directories(Crabs PRIVATE
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_SOURCE_DIR}/_Seams"
)
