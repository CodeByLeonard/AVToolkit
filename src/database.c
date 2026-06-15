#include "database.h"
#include <stdio.h>

sqlite3* db_open(void) {
    sqlite3 *db;
    if (sqlite3_open("avtoolkit.db", &db)) {
        printf("DB error\n");
        return NULL;
    }
    return db;
}

void db_init(sqlite3 *db) {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS sessions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT,"
        "shorts_id TEXT,"
        "medium_path TEXT,"
        "created_at INTEGER"
        ");"

        "CREATE TABLE IF NOT EXISTS segments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "session_id INTEGER,"
        "start_ms INTEGER,"
        "end_ms INTEGER"
        ");";

    sqlite3_exec(db, sql, 0, 0, 0);
}