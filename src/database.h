#pragma once
#include <sqlite3.h>

sqlite3* db_open(void);
void db_init(sqlite3 *db);
