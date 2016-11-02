# Find Criterion for Unit Testing
find_package(Check REQUIRED)
include_directories(
    ${CHECK_INCLUDE_DIR}
    ${TEST_SOURCE_DIR}/src
)
set(LIBS ${LIBS} ${CHECK_LIBRARY} Milight)


add_executable(TestCRC crc.c)
target_link_libraries(TestCRC ${LIBS})

add_executable(TestMilight milight.c)
target_link_libraries(TestMilight ${LIBS})
