# Добавляем исполняемый файл отправителя
add_executable(sender
    sender.cpp
)

# Связываем с общей библиотекой
target_link_libraries(sender PRIVATE message_file)