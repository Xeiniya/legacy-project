# src/sender/CMakeLists.txt
add_executable(sender
    sender.cpp
    sender.h
)

target_link_libraries(sender PRIVATE message_file)

# Make sure sender can find the common directory
target_include_directories(sender PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/common
)

# Debug information about sender build
message(STATUS "Sender output path: $<TARGET_FILE:sender>")

# Для Windows добавляем подсистему консоли
if(WIN32)
    target_link_options(sender PRIVATE "/SUBSYSTEM:CONSOLE")
    # Ensure we link against winsock if needed
    target_link_libraries(sender PRIVATE ws2_32)
endif()

# For cross-platform development, ensure c++98 compliance
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(sender PRIVATE -Wall -Wextra -pedantic)
endif()