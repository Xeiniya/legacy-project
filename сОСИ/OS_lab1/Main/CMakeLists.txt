
# Добавьте источник в исполняемый файл этого проекта.

# Main/CMakeLists.txt
add_executable(Main main.cpp )
target_include_directories(Main PRIVATE ${PROJECT_SOURCE_DIR}/include)

