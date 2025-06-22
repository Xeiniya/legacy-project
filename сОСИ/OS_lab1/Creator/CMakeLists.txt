# Creator/CMakeLists.txt
add_executable(Creator creator.cpp)
target_include_directories(Creator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Creator PRIVATE ${PROJECT_SOURCE_DIR}/include)
