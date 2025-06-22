cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("lab2")

add_executable (lab2 "src/main.cpp"  "src/Threads.h" "src/Threads.cpp" "src/Data.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab2 PROPERTY CXX_STANDARD 20)
endif()

enable_testing()
add_subdirectory(tests)
