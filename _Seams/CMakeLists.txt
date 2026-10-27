cmake_minimum_required (VERSION 2.6)
project (Crabs)

# Single-TU rule: one .cpp = one .exe. The entry lives in _Main.hxx (this
# directory); the generated .cpp wrapper (build tree) is the only source.
file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
file (WRITE "${CMAKE_BINARY_DIR}/generated/_Main.cpp"
  "#include \"${CMAKE_CURRENT_SOURCE_DIR}/_Main.hxx\"\n")
add_executable(Crabs "${CMAKE_BINARY_DIR}/generated/_Main.cpp")
