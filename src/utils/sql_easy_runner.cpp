#include <iostream>
#include "sql_easy_runner.h"

namespace colnago
{
    namespace utils
    {
        namespace sql_easy_runner
        {

            std::string escap_char(const std::string &str)
            {
                std::string res = str;
                long int i = 0;
                long int size = res.size();
                while (i < size)
                {
                    if (res[i] == '\"' && i != 0 && res[i - 1] != '\\')
                    {
                        res.insert(i, 1, '\\');
                        i++;
                    }
                    else if (res[i] == '\'' && i != 0 && res[i - 1] != '\'')
                    {
                        res.insert(i, 1, '\'');
                        i++;
                    }
                    else if (res[i] == '\\' && i != 0 && res[i - 1] != '\\')
                    {
                        res.insert(i, 1, '\\');
                        i++;
                    }
                    else if (res[i] == '&' && i + 3 <= size - 1 && res[i + 1] == 'l' && res[i + 2] == 't' && res[i + 3] == ';')
                    {
                        res.erase(i, 4);
                        res.insert(i, "<");
                    }
                    else if (res[i] == '&' && i + 3 <= size - 1 && res[i + 1] == 'g' && res[i + 2] == 't' && res[i + 3] == ';')
                    {
                        res.erase(i, 4);
                        res.insert(i, ">");
                    }

                    i++;
                    size = res.size();
                }
                return res;
            }

            std::string parse_escap_char(const std::string &str)
            {
                std::string res = str;
                long int i = 0;
                long int size = res.size();
                while (i < size)
                {
                    if (res[i] == '\'' && i < size - 1 && res[i + 1] == '\'')
                    {
                        res.erase(i, 1);
                    }
                    else if (res[i] == '\\' && i < size - 1 && res[i + 1] == '\"')
                    {
                        res.erase(i, 1);
                    }
                    else if (res[i] == '\\' && i < size - 1 && res[i + 1] == '\\')
                    {
                        res.erase(i, 1);
                    }
                    else if (res[i] == '<')
                    {                    //&lt;
                        res.erase(i, 1); // delete <
                        res.insert(i, "&lt;");
                        i += 3;
                    }
                    else if (res[i] == '>')
                    {                    //&gt;
                        res.erase(i, 1); // delete <
                        res.insert(i, "&gt;");
                        i += 3;
                    }
                    size = res.size();
                    i++;
                }
                return res;
            }

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
