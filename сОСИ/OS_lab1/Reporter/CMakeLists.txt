# Reporter/CMakeLists.txt
add_executable(Reporter reporter.cpp)
target_include_directories(Reporter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Reporter PRIVATE ${PROJECT_SOURCE_DIR}/include)
# Включаем директорию Creator в пути для поиска заголовочных файлов
target_include_directories(Reporter PRIVATE ${PROJECT_SOURCE_DIR}/Creator)
