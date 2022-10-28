#include "dao/init_db/init_db.h"

namespace colnago
{
    namespace dao
    {
        ColnagoDB::ColnagoDB(const char *path)
        {
            int rc = sqlite3_open(path, &db);
            if (rc)
            {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                exit(0);
            }
            fprintf(stdout, "opened database: %s\n", sqlite3_errmsg(db));
        }

        ColnagoDB::~ColnagoDB()
        {
            if (db)
            {
                sqlite3_close(db);
            }
        }

        void ColnagoDB::init()
        {
            const char *sql = "CREATE TABLE IF NOT EXISTS post("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                              "TEXT MEMO NOT NULL,"
                              "DATE DATETIME NOT NULL);"
                              "CREATE TABLE  IF NOT EXISTS image("
                              "ID VARCHAR(32) PRIMARY KEY NOT NULL,"
                              "DATA BLOB NOT NULL,"
                              "TYPE VARCHAR(32) NOT NULL"
                              ");";
            // execute sql
            char *zErrMsg = nullptr;
            int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
            if (rc != SQLITE_OK)
            {
                std::cout << "sqlite error:: " << std::string(zErrMsg) << std::endl;
                sqlite3_free(zErrMsg);
            }
            else
            {
                std::cout << "sqlite info:: "
                          << "table created successfully" << std::endl;
            }
        }

    }
}