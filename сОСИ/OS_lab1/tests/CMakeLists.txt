# Enable testing (if not already enabled in parent CMakeLists.txt)
enable_testing()

# Add executable files for tests
add_executable(test_main test_main.cpp)
add_executable(test_creator test_creator.cpp)
add_executable(test_reporter test_reporter.cpp)

# Link with GoogleTest libraries
target_link_libraries(test_main PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(test_creator PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(test_reporter PRIVATE GTest::gtest GTest::gtest_main)

# Include headers from project
target_include_directories(test_creator PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_reporter PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_main PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Include GoogleTest discovery
include(GoogleTest)

# Auto-discover tests
gtest_discover_tests(test_main)
gtest_discover_tests(test_creator)
gtest_discover_tests(test_reporter)

# No need for add_test() when using gtest_discover_tests()