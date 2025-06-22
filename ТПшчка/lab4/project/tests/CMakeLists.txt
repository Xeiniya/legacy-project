# Find Google Test package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Define test files
set(TEST_FILES
    database_test.cpp
    authentication_test.cpp
    test_main.cpp
)

# Test source files (excluding main.cpp)
set(TEST_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/database.cpp
    ${CMAKE_SOURCE_DIR}/src/authentication.cpp
)

# Copy database file for tests
configure_file(${CMAKE_SOURCE_DIR}/data/flower.db ${CMAKE_CURRENT_BINARY_DIR}/flower.db COPYONLY)

# Add test executable
add_executable(run_tests ${TEST_FILES} ${TEST_SOURCE_FILES})
target_include_directories(run_tests PRIVATE ${SQLite3_INCLUDE_DIR})
target_link_libraries(run_tests PRIVATE ${GTEST_LIBRARIES} pthread ${SQLite3_LIBRARY})


# Register tests
add_test(NAME UnitTests COMMAND run_tests)


