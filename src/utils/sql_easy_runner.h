#pragma once
#include <sstream>
#include <sqlite3.h>
#include <string>
namespace colnago
{
    namespace utils
    {
        namespace sql_easy_runner
        {
            using namespace std;
            std::string escap_char(const std::string& str);
            std::string parse_escap_char(const std::string& str);
            std::pair<bool, std::string> esay_run(const std::stringstream &sql, sqlite3 *db);
            std::pair<bool, std::string> callback_run(const stringstream &sql, sqlite3 *db, int (*callback)(void *data, int argc, char **argv, char **azColName), void *data);
        }
    }
} // namespace colnago
