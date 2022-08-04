#include "user.h"
#include <sstream>
#include "utils/sql_easy_runner.h"
#include <cstdio>
#include <map>
#include <string>
#include <iostream>

namespace colnago
{
    namespace dao
    {
        std::ostream &operator<<(std::ostream &os, const User &user)
        {
            os << user.id << " " << user.name << " " << user.num;
            return os;
        }

        std::pair<bool, std::string> UserDao::INSERT(const User &user)
        {
            std::stringstream sql;
            sql << "INSERT INTO USER (ID,NAME,NUM) ";
            sql << "VALUES (";
            sql << user.id
                << ","
                << "'" << user.name << "'"
                << ","
                << user.num << ");";
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::pair<bool, std::string> UserDao::DELETE(const User &user)
        {
            std::stringstream sql;
            sql << "DELETE FROM USER WHERE ID=" << user.id;
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::pair<bool, std::string> UserDao::UPDATE(const User &user)
        {
            std::stringstream sql;
            sql << " UPDATE USER SET NAME = "
                << "'" << user.name << "'"
                << ","
                << " NUM = " << user.num
                << " WHERE ID = " << user.id << ";";
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::tuple<bool, std::string, std::list<User>> UserDao::SELECT(const User &user)
        {
            std::stringstream sql;
            if (user.id == 0)
                sql << "SELECT * FROM USER;";
            else
                sql << "SELECT * FROM USER WHERE ID=" << user.id << ";";
            std::list<User> users;
            //回调函数
            auto callback = [](void *data, int argc, char **argv, char **azColName) mutable -> int
            {
                std::list<User> *users_ptr = (std::list<User> *)data;
                std::list<User> &users = *users_ptr;
                std::map<std::string, std::string> m_map;
                for (int i = 0; i < argc; i++)
                {
                    User new_user;
                    std::string key(azColName[i]);
                    std::string value(argv[i]);
                    m_map[key] = value;
                }
                User m_user;
                m_user.id = stoi(m_map["ID"]);
                m_user.name = m_map["NAME"];
                m_user.num = stoi(m_map["NUM"]);
                users.push_back(m_user);
                return 0;
            };
            std::list<User> *users_ptr = &users;
            auto res_pair = colnago::utils::sql_easy_runner::callback_run(sql, db, callback, (void *)users_ptr);
            std::tuple<bool, std::string, std::list<User>> res = std::make_tuple(res_pair.first, res_pair.second, users);
            return res;
        }
    }
}