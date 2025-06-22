cmake_minimum_required(VERSION 3.5)
project(message_passing)

# Настройки компилятора для C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable C++ extensions to ensure strict standard compliance
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # Disable Windows API name conflicts
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # For MSVC, we need to explicitly set C++ standard
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++98")
endif()

# For GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -pedantic")
endif()

# Включаем автоматическое добавление текущих директорий
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Output directory configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Make sure executable files are placed in the same directory
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(src/common)  
add_subdirectory(src/receiver)
add_subdirectory(src/sender)

# Add extra debug information
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Copy dependencies to the build directory
if(WIN32)
    add_custom_target(copy_binaries ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying binaries to ${CMAKE_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sender> ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:receiver> ${CMAKE_BINARY_DIR}/bin
        DEPENDS sender receiver
    )
endif()

# Тестирование
include(CTest)
enable_testing()

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()