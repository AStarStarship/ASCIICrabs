cmake_minimum_required (VERSION 3.10)
project (ASCIICrabs)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Single-TU rule: one .cpp = one .exe/.dll. The source tree holds no .cpp
# implementation files — the entry point lives in _Seams/_Main.hxx and the
# generated .cpp wrapper below (build tree only) is the only source of the
# executable; it pulls in the .hxx entry and its _Package.hxx rollup.
file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
file (WRITE "${CMAKE_BINARY_DIR}/generated/_Main.cpp"
  "#include \"${CMAKE_CURRENT_SOURCE_DIR}/_Seams/_Main.hxx\"\n")

add_executable(ASCIICrabs "${CMAKE_BINARY_DIR}/generated/_Main.cpp")

target_include_directories(ASCIICrabs PRIVATE
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_SOURCE_DIR}/_Seams"
)

# NOTE: the RoomBindingTests target was removed 2026-10-27 — its source
# _Seams/RoomBinding.cpp was deleted in 598d757 and the stale reference
# blocked every fresh CMake configure ("Cannot find source file").
