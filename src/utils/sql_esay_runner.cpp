#include "sql_easy_runner.h"
namespace colnago
{
    namespace utils
    {
        namespace sql_easy_runner
        {
            std::pair<bool, std::string> esay_run(const stringstream &sql, sqlite3 *db)
            {
                char *zErrMsg = nullptr;
                int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, 0, &zErrMsg);
                std::string message = "";
                if (zErrMsg)
                {
                    message = zErrMsg;
                }
                std::pair<bool, std::string> res((rc == SQLITE_OK), message);
                sqlite3_free(zErrMsg);
                return res;
            }
            std::pair<bool, std::string> callback_run(const stringstream &sql, sqlite3 *db, int (*callback)(void *data, int argc, char **argv, char **azColName), void *data)
            {
                char *zErrMsg = nullptr;
                int rc = sqlite3_exec(db, sql.str().c_str(), callback, data, &zErrMsg);
                std::string message = "";
                if (zErrMsg)
                {
                    message = zErrMsg;
                }
                std::pair<bool, std::string> res((rc == SQLITE_OK), message);
                sqlite3_free(zErrMsg);
                return res;
            }
        }
    }
} // namespace colnago