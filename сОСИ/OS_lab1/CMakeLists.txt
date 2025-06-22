cmake_minimum_required(VERSION 3.8)

# Set project name
project("Laba1")

# Include the headers directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Hot reload for MSVC compilers if supported
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Enable testing
include(CTest)
enable_testing()

# Fetch and configure GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0  # Using a more recent version
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include GoogleTest module for test discovery
include(GoogleTest)

# Add the executables for tests first since we'll need to reference them
add_executable(test_creator tests/test_creator.cpp "include/Structure.h")
add_executable(test_reporter tests/test_reporter.cpp "include/Structure.h")
add_executable(test_main tests/test_main.cpp "include/Structure.h")

# Link the test executables with GoogleTest
target_link_libraries(test_creator PRIVATE gtest gtest_main)
target_link_libraries(test_reporter PRIVATE gtest gtest_main)
target_link_libraries(test_main PRIVATE gtest gtest_main)

# Make sure tests can find the include directory
target_include_directories(test_creator PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_reporter PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test_main PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Register tests with CTest
gtest_discover_tests(test_creator)
gtest_discover_tests(test_reporter)
gtest_discover_tests(test_main)

# Include subdirectories for main components
add_subdirectory(Main)
add_subdirectory(Creator)
add_subdirectory(Reporter)