cmake_minimum_required(VERSION 3.14)
project(message_passing)

# Установка стандарта C++ и опций компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение поддержки тестирования
include(CTest)
enable_testing()

# Добавление подпроектов
add_subdirectory(src/receiver)
add_subdirectory(src/sender)

# Добавление тестов, если включено тестирование
if(BUILD_TESTING)
    # Загрузка GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()