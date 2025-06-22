cmake_minimum_required(VERSION 3.10)
project(FlowerShop VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Define source files
set(SOURCE_FILES
    src/main.cpp
    src/database.cpp
    src/authentication.cpp
    src/ui.cpp
)

# Main executable
add_executable(flower_shop ${SOURCE_FILES})
target_include_directories(flower_shop PRIVATE ${SQLite3_INCLUDE_DIR})
target_link_libraries(flower_shop PRIVATE ${SQLite3_LIBRARY})


# Install executable
install(TARGETS flower_shop DESTINATION bin)

# Copy database file to build directory
configure_file(data/flower.db ${CMAKE_BINARY_DIR}/flower.db COPYONLY)

# Testing
add_subdirectory(tests)
