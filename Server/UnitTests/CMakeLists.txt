cmake_minimum_required(VERSION 3.14)
project(UnitTests)

enable_testing()

add_executable(
  ${PROJECT_NAME}
  chat_member_test.cpp
  socket_test.cpp
  logger_test.cpp
)
target_link_libraries(
  ${PROJECT_NAME}
  gmock
  GTest::gtest_main
  tests
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

