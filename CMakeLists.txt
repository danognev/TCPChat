cmake_minimum_required (VERSION 3.12)
project("TcpChat")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin")

add_subdirectory ("Server")