#pragma once
#include <string>
#include "dao/init_db.h"
#include <sqlite3.h>
#include <list>
#include <iostream>
#include <tuple>
namespace colnago
{
    namespace dao
    {
        class User
        {
        public:
            int id;
            std::string name;
            int num;
            User(const int &id, const std::string &name, const int &num) : id(id), name(name), num(num){};
            User() = default;
            friend std::ostream &operator<<(std::ostream &os, const User &user);
        };
        std::ostream &operator<<(std::ostream &os, const User &user);

        class UserDao
        {
        private:
            sqlite3 *db;

        public:
            UserDao(sqlite3 *db) : db(db)
            {
            }
            std::pair<bool, std::string> INSERT(const User &user);
            std::pair<bool, std::string> DELETE(const User &user);
            std::pair<bool, std::string> UPDATE(const User &user);
            std::tuple<bool, std::string, std::list<User>> SELECT(const User &user);
        };
    }
}