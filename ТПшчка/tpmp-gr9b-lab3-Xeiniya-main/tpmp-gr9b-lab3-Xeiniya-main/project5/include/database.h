#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

/* 
 * Устанавливает соединение с базой данных
 * @param db - указатель на указатель базы данных SQLite
 * @return - код ошибки SQLite (0 в случае успеха)
 */
int connect_db(sqlite3 **db);

/*
 * Закрывает соединение с базой данных
 * @param db - указатель на базу данных SQLite
 */
void close_db(sqlite3 *db);

/*
 * Вставляет новую запись в таблицу car_owners в режиме autocommit
 * @param db - указатель на базу данных SQLite
 * @return - код ошибки (0 в случае успеха)
 */
int insert_autocommit(sqlite3 *db);

/*
 * Вставляет новую запись в таблицу car_owners с использованием транзакции
 * @param db - указатель на базу данных SQLite
 * @return - код ошибки (0 в случае успеха)
 */
int insert_transaction(sqlite3 *db);

/*
 * Выбирает запись по идентификатору владельца
 * @param db - указатель на базу данных SQLite
 * @param id - идентификатор владельца
 * @return - код ошибки (0 в случае успеха)
 */
int select_by_id(sqlite3 *db, int id);

/*
 * Выбирает записи по шаблону фамилии (с использованием LIKE)
 * @param db - указатель на базу данных SQLite
 * @param pattern - шаблон для поиска по фамилии
 * @return - код ошибки (0 в случае успеха)
 */
int select_by_lastname_pattern(sqlite3 *db, const char *pattern);

/*
 * Выбирает записи по названию города
 * @param db - указатель на базу данных SQLite
 * @param city - название города
 * @return - код ошибки (0 в случае успеха)
 */
int select_by_city(sqlite3 *db, const char *city);

/*
 * Удаляет запись по идентификатору владельца
 * @param db - указатель на базу данных SQLite
 * @param id - идентификатор владельца
 * @return - код ошибки (0 в случае успеха)
 */
int delete_by_id(sqlite3 *db, int id);

/*
 * Извлекает фотографию владельца и сохраняет её в файл
 * @param db - указатель на базу данных SQLite
 * @param id - идентификатор владельца
 * @param filename - имя файла для сохранения фотографии
 * @return - код ошибки (0 в случае успеха)
 */
int extract_photo_to_file(sqlite3 *db, int id, const char *filename);

/*
 * Вставляет новую запись владельца с фотографией из файла
 * @param db - указатель на базу данных SQLite
 * @param photo_path - путь к файлу с фотографией
 * @return - код ошибки (0 в случае успеха)
 */
int insert_with_photo(sqlite3 *db, const char *photo_path);

#endif
