#include "user.h"
#include <sstream>
#include "utils/sql_easy_runner.h"
#include <cstdio>
#include <map>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "inja/inja.hpp"

namespace colnago
{
    namespace dao
    {
        using namespace std;
        std::ostream &operator<<(std::ostream &os, const User &user)
        {
            os << user.id << " " << user.name << " " << user.num;
            return os;
        }

        std::string User::stringify()
        {
            using json = nlohmann::json;
            json j_object = {{"id", id}, {"num", num}, {"name", name}};
            return j_object.dump();
        }

        void User::parse(const std::string &json_str)
        {
            using json = nlohmann::json;
            json json_obj = json::parse(json_str);
            id = json_obj["id"];
            name = json_obj["name"];
            num = json_obj["num"];
        }

        std::pair<bool, std::string> UserDao::INSERT(User &user)
        {
            nlohmann::json data = nlohmann::json::parse(user.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(
                INSERT INTO USER (NAME,NUM) 
                VALUES ('{{ name }}',{{ num }});)",
                            data);
            // cout << sql.str() << endl;
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::pair<bool, std::string> UserDao::DELETE(User &user)
        {
            nlohmann::json data = nlohmann::json::parse(user.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(DELETE FROM USER WHERE ID = {{ id }})", data);
            // cout << sql.str() << endl;
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::pair<bool, std::string> UserDao::UPDATE(User &user)
        {
            nlohmann::json data = nlohmann::json::parse(user.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(
                UPDATE USER SET 
                NAME = '{{ name }}',NUM = {{ num }}
                WHERE ID = {{ id }})",
                            data);
            // cout << sql.str() << endl;
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::tuple<bool, std::string, std::list<User>> UserDao::SELECT(User &user)
        {
            nlohmann::json data = nlohmann::json::parse(user.stringify());
            std::stringstream sql;
            if (user.id == 0)
                inja::render_to(sql, R"(SELECT * FROM USER ORDER BY ID;)", data);
            else
                inja::render_to(sql, R"(SELECT * FROM USER WHERE ID = {{ id }} ORDER BY ID;)", data);
            // cout << sql.str() << endl;
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