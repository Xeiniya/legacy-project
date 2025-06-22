# src/common/CMakeLists.txt
add_library(message_file STATIC
    message_file.cpp
    message_file.h
)

# Make headers available to all projects that link to message_file
target_include_directories(message_file PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# For Windows, ensure we link against necessary libraries
if(WIN32)
    target_link_libraries(message_file PUBLIC ws2_32)
endif()

# For Linux/Unix, link against pthread and rt for semaphore support
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(message_file PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    # Link against librt for semaphore support
    target_link_libraries(message_file PUBLIC rt)
endif()

# Ensure C++98 compliance
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(message_file PRIVATE -Wall -Wextra -pedantic)
endif()