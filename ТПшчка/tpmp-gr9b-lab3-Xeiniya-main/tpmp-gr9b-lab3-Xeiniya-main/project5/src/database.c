#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

/**
 * Устанавливает соединение с базой данных
 * 
 * @param db Указатель на указатель базы данных
 * @return Код ошибки SQLite (0 в случае успеха)
 */
int connect_db(sqlite3 **db) {
    // Открываем соединение с файлом базы данных
    return sqlite3_open("db/car_database.db", db);
}

/**
 * Закрывает соединение с базой данных
 * 
 * @param db Указатель на базу данных
 */
void close_db(sqlite3 *db) {
    // Закрываем соединение с базой данных
    sqlite3_close(db);
}

/**
 * Вставляет новую запись в режиме autocommit
 * В этом режиме каждая SQL инструкция выполняется как отдельная транзакция
 * 
 * @param db Указатель на базу данных
 * @return Код ошибки (0 в случае успеха)
 */
int insert_autocommit(sqlite3 *db) {
    // SQL запрос для вставки данных
    const char *sql = "INSERT INTO car_owners (last_name, first_name, middle_name, phone_number, country, city, car_brand, car_number, tech_passport, price, manufacture_year) "
                      "VALUES ('Иванов', 'Иван', 'Иванович', '+79991234567', 'Россия', 'Москва', 'ВАЗ', 'A123BC77', '1234567890', 300000, 2010);";
    char *err = NULL;
    
    // Выполняем SQL запрос
    int rc = sqlite3_exec(db, sql, 0, 0, &err);
    
    // Проверяем результат выполнения
    if (rc != SQLITE_OK) {
        printf("Ошибка SQL: %s\n", err);
        sqlite3_free(err);
        return rc;
    }
    
    printf("Запись добавлена (autocommit)\n");
    return 0;
}

/**
 * Вставляет новую запись с использованием явной транзакции
 * Это позволяет откатить изменения в случае ошибки
 * 
 * @param db Указатель на базу данных
 * @return Код ошибки (0 в случае успеха)
 */
int insert_transaction(sqlite3 *db) {
    char *err = NULL;
    int rc;
    
    // Начинаем транзакцию
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        printf("Ошибка начала транзакции: %s\n", err);
        sqlite3_free(err);
        return rc;
    }
    
    // SQL запрос для вставки данных
    const char *sql = "INSERT INTO car_owners (last_name, first_name, middle_name, phone_number, country, city, car_brand, car_number, tech_passport, price, manufacture_year) "
                      "VALUES ('Петров', 'Петр', 'Петрович', '+79997654321', 'Россия', 'Санкт-Петербург', 'Lada', 'B321CB77', '9876543210', 400000, 2015);";
    
    // Выполняем SQL запрос в рамках транзакции
    rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        printf("Ошибка SQL в транзакции: %s\n", err);
        sqlite3_free(err);
        // В случае ошибки откатываем транзакцию
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return rc;
    }
    
    // Фиксируем изменения транзакции
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        printf("Ошибка завершения транзакции: %s\n", err);
        sqlite3_free(err);
        return rc;
    }
    
    printf("Запись добавлена (транзакция)\n");
    return 0;
}

/**
 * Выполняет поиск владельца по его идентификатору
 * 
 * @param db Указатель на базу данных
 * @param id Идентификатор владельца
 * @return Код ошибки (0 в случае успеха)
 */
int select_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    // SQL запрос с параметром для поиска по ID
    const char *sql = "SELECT id, last_name, first_name, middle_name, car_brand, car_number, manufacture_year FROM car_owners WHERE id = ?";
    
    // Подготавливаем SQL запрос
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Привязываем параметр к запросу
    sqlite3_bind_int(stmt, 1, id);
    
    // Выполняем запрос и обрабатываем результаты
    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        printf("ID: %d, Фамилия: %s, Имя: %s, Отчество: %s\n"
               "Автомобиль: %s, Номер: %s, Год выпуска: %d\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 4),
               sqlite3_column_text(stmt, 5),
               sqlite3_column_int(stmt, 6));
    }
    
    // Сообщаем, если запись не найдена
    if (!found) {
        printf("Запись с ID=%d не найдена\n", id);
    }
    
    // Освобождаем ресурсы подготовленного запроса
    sqlite3_finalize(stmt);
    return 0;
}

/**
 * Выполняет поиск владельцев по шаблону фамилии
 * Использует SQL оператор LIKE с подстановочными символами %
 * 
 * @param db Указатель на базу данных
 * @param pattern Шаблон для поиска (фрагмент фамилии)
 * @return Код ошибки (0 в случае успеха)
 */
int select_by_lastname_pattern(sqlite3 *db, const char *pattern) {
    sqlite3_stmt *stmt;
    // SQL запрос с параметром LIKE для поиска по шаблону
    const char *sql = "SELECT id, last_name, first_name, middle_name, car_brand FROM car_owners WHERE last_name LIKE ?";
    
    // Подготавливаем SQL запрос
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Формируем шаблон для SQL LIKE (добавляем % с обеих сторон)
    char like_pattern[100];
    snprintf(like_pattern, sizeof(like_pattern), "%%%s%%", pattern);
    
    // Привязываем параметр к запросу
    sqlite3_bind_text(stmt, 1, like_pattern, -1, SQLITE_TRANSIENT);
    
    // Выполняем запрос и выводим результаты
    int count = 0;
    printf("Результаты поиска по фамилии '%s':\n", pattern);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        printf("ID: %d, %s %s %s, Авто: %s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 4));
    }
    
    // Выводим итоговую информацию о результатах поиска
    if (count == 0) {
        printf("Записей не найдено\n");
    } else {
        printf("Всего найдено: %d\n", count);
    }
    
    // Освобождаем ресурсы подготовленного запроса
    sqlite3_finalize(stmt);
    return 0;
}

/**
 * Выполняет поиск владельцев по названию города
 * 
 * @param db Указатель на базу данных
 * @param city Название города
 * @return Код ошибки (0 в случае успеха)
 */
int select_by_city(sqlite3 *db, const char *city) {
    sqlite3_stmt *stmt;
    // SQL запрос с параметром для поиска по городу
    const char *sql = "SELECT id, last_name, first_name, city, car_brand, car_number FROM car_owners WHERE city = ?";
    
    // Подготавливаем SQL запрос
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Привязываем параметр к запросу
    sqlite3_bind_text(stmt, 1, city, -1, SQLITE_TRANSIENT);
    
    // Выполняем запрос и выводим результаты
    int count = 0;
    printf("Владельцы автомобилей в городе '%s':\n", city);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        printf("ID: %d, %s %s, Город: %s, Авто: %s, Номер: %s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 4),
               sqlite3_column_text(stmt, 5));
    }
    
    // Выводим итоговую информацию о результатах поиска
    if (count == 0) {
        printf("Записей не найдено\n");
    } else {
        printf("Всего найдено: %d\n", count);
    }
    
    // Освобождаем ресурсы подготовленного запроса
    sqlite3_finalize(stmt);
    return 0;
}

/**
 * Удаляет запись владельца по идентификатору
 * Сначала проверяет существование записи, затем удаляет
 * 
 * @param db Указатель на базу данных
 * @param id Идентификатор владельца
 * @return Код ошибки (0 в случае успеха)
 */
int delete_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    
    // Сначала проверяем, существует ли запись с таким ID
    const char *check_sql = "SELECT id FROM car_owners WHERE id = ?";
    int rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Привязываем параметр к запросу
    sqlite3_bind_int(stmt, 1, id);
    
    // Проверяем, существует ли запись
    int exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    
    if (!exists) {
        printf("Запись с ID=%d не найдена\n", id);
        return 1;
    }
    
    // Теперь удаляем запись
    const char *sql = "DELETE FROM car_owners WHERE id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Привязываем параметр к запросу
    sqlite3_bind_int(stmt, 1, id);
    
    // Выполняем запрос
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Проверяем результат выполнения
    if (rc == SQLITE_DONE) {
        printf("Запись с ID=%d успешно удалена\n", id);
        return 0;
    } else {
        printf("Ошибка при удалении записи: %s\n", sqlite3_errmsg(db));
        return 1;
    }
}

/**
 * Извлекает фотографию владельца из базы данных и сохраняет в файл
 * 
 * @param db Указатель на базу данных
 * @param id Идентификатор владельца
 * @param filename Имя файла для сохранения фотографии
 * @return Код ошибки (0 в случае успеха)
 */
int extract_photo_to_file(sqlite3 *db, int id, const char *filename) {
    sqlite3_stmt *stmt;
    // SQL запрос для извлечения фотографии
    const char *sql = "SELECT photo FROM car_owners WHERE id = ?";
    
    // Подготавливаем SQL запрос
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    // Привязываем параметр к запросу
    sqlite3_bind_int(stmt, 1, id);
    
    // Выполняем запрос и обрабатываем результат
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Получаем данные BLOB (фотографию)
        const void *blob = sqlite3_column_blob(stmt, 0);
        int size = sqlite3_column_bytes(stmt, 0);
        
        // Проверяем, есть ли данные
        if (size == 0 || blob == NULL) {
            printf("У владельца с ID=%d нет фотографии\n", id);
            sqlite3_finalize(stmt);
            return 1;
        }
        
        // Открываем файл для записи в двоичном режиме
        FILE *fp = fopen(filename, "wb");
        if (fp) {
            // Записываем данные в файл
            fwrite(blob, 1, size, fp);
            fclose(fp);
            printf("Фотография успешно сохранена в файл: %s\n", filename);
        } else {
            printf("Ошибка при открытии файла для записи\n");
            sqlite3_finalize(stmt);
            return 1;
        }
    } else {
        printf("Владелец с ID=%d не найден\n", id);
        sqlite3_finalize(stmt);
        return 1;
    }
    
    // Освобождаем ресурсы подготовленного запроса
    sqlite3_finalize(stmt);
    return 0;
}

/**
 * Добавляет нового владельца с фотографией из файла
 * 
 * @param db Указатель на базу данных
 * @param photo_path Путь к файлу с фотографией
 * @return Код ошибки (0 в случае успеха)
 */
int insert_with_photo(sqlite3 *db, const char *photo_path) {
    sqlite3_stmt *stmt;
    FILE *fp;
    long file_size;
    char *buffer;
    
    // Открываем файл с фотографией в двоичном режиме
    fp = fopen(photo_path, "rb");
    if (!fp) {
        printf("Не удалось открыть файл с фотографией: %s\n", photo_path);
        return 1;
    }
    
    // Определяем размер файла
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Выделяем память под буфер для хранения содержимого файла
    buffer = (char*)malloc(file_size);
    if (!buffer) {
        printf("Ошибка выделения памяти\n");
        fclose(fp);
        return 1;
    }
    
    // Читаем содержимое файла в буфер
    fread(buffer, file_size, 1, fp);
    fclose(fp);
    
    // Готовим SQL запрос с параметризацией для вставки данных включая BLOB
    const char *sql = "INSERT INTO car_owners (last_name, first_name, middle_name, phone_number, country, city, car_brand, car_number, tech_passport, price, manufacture_year, photo) "
                     "VALUES ('Сидоров', 'Алексей', 'Сидорович', '+79993456789', 'Россия', 'Казань', 'Toyota', 'C456DE77', '5678901234', 800000, 2018, ?);";
    
    // Подготавливаем SQL запрос
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        free(buffer);
        return rc;
    }
    
    // Привязываем BLOB (фотографию) к параметру запроса
    sqlite3_bind_blob(stmt, 1, buffer, file_size, SQLITE_STATIC);
    
    // Выполняем запрос
    rc = sqlite3_step(stmt);
    
    // Проверяем результат выполнения
    if (rc != SQLITE_DONE) {
        printf("Ошибка выполнения запроса: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        free(buffer);
        return rc;
    }
    
    printf("Запись с фотографией успешно добавлена\n");
    
    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
    free(buffer);
    return 0;
}
