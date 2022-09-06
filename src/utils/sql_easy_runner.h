#pragma once
#include <sstream>
#include <sqlite3.h>
#include <string>
#include <list>
#include <functional>
#include <tuple>
#include <map>

namespace colnago
{
    namespace utils
    {
        namespace sql_easy_runner
        {
            using namespace std;
            std::string escap_char(const std::string &str);
            std::string parse_escap_char(const std::string &str);
            std::pair<bool, std::string> esay_run(const std::stringstream &sql, sqlite3 *db);
            std::pair<bool, std::string> callback_run(const stringstream &sql, sqlite3 *db, int (*callback)(void *data, int argc, char **argv, char **azColName), void *data);

            // callback_run 回调
            template <typename T>
            class CallbackTolist
            {
            public:
                static std::tuple<bool, std::string, std::list<T>> f(const stringstream &sql, sqlite3 *db);

            private:
                static int format(void *data, int argc, char **argv, char **azColName);
            };

            template <typename T>
            int CallbackTolist<T>::format(void *data, int argc, char **argv, char **azColName)
            {
                std::list<T> *items_ptr = (std::list<T> *)data;
                std::list<T> &items = *items_ptr;
                std::map<std::string, std::string> m_map;
                for (int i = 0; i < argc; i++)
                {
                    std::string key(azColName[i]);
                    std::string value(argv[i]);
                    m_map[key] = value;
                }
                T m_item(m_map);
                items.push_back(m_item);
                return 0;
            }

            template <typename T>
            std::tuple<bool, std::string, std::list<T>> CallbackTolist<T>::f(const stringstream &sql, sqlite3 *db)
            {
                std::list<T> items;
                decltype(items) *items_ptr = &items;
                auto res_pair = callback_run(sql, db, format, (void *)items_ptr);
                return std::make_tuple(res_pair.first, res_pair.second, items);
            }
        }
    }
} // namespace colnago
