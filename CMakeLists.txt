cmake_minimum_required (VERSION 2.6)
project (MilightTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/.cmake/Modules/)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME TestCRC COMMAND TestCRC)
add_test(NAME TestMilight COMMAND TestMilight)
set_tests_properties(TestMilight PROPERTIES DEPENDS "TestCRC")