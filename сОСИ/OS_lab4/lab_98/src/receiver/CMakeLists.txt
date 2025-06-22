# src/receiver/CMakeLists.txt
add_executable(receiver
    receiver.cpp
    receiver.h
)

target_link_libraries(receiver PRIVATE message_file)

# Указываем правильные пути для include
target_include_directories(receiver PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/common
)

# Enable C++98 filesystem alternative libraries if needed
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # For older compilers you might need to link with librt for semaphores
    target_link_libraries(receiver PRIVATE rt)
endif()

# Debug information about receiver build
message(STATUS "Receiver output path: $<TARGET_FILE:receiver>")

# Для Windows добавляем подсистему консоли
if(WIN32)
    target_link_options(receiver PRIVATE "/SUBSYSTEM:CONSOLE")
    # Ensure we link against winsock if needed
    target_link_libraries(receiver PRIVATE ws2_32)
endif()

# For cross-platform development, ensure c++98 compliance
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(receiver PRIVATE -Wall -Wextra -pedantic)
endif()