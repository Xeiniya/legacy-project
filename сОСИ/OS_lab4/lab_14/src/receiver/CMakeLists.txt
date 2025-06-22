# Добавляем библиотеку для общего кода
add_library(message_file STATIC
    ../common/message_file.h
    ../common/message_file.cpp
)

# Настраиваем пути включения для библиотеки
target_include_directories(message_file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)

# Добавляем исполняемый файл приемника
add_executable(receiver
    receiver.cpp
)

# Связываем с общей библиотекой
target_link_libraries(receiver PRIVATE message_file)