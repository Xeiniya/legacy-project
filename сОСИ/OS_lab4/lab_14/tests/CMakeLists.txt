# Добавляем тесты сообщений
add_executable(message_file_test
    message_file_test.cpp
)
target_link_libraries(message_file_test
    PRIVATE message_file
    PRIVATE GTest::gtest_main
)

# Добавляем тест в список тестирования
add_test(NAME message_file_test COMMAND message_file_test)

# Настраиваем пути включения для тестов
target_include_directories(message_file_test PRIVATE ${CMAKE_SOURCE_DIR}/src)