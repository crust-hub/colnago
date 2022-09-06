#pragma once
#include <string>
#include <sqlite3.h>
#include <list>
#include <iostream>
#include <tuple>
#include <map>

namespace colnago
{
    namespace dao
    {
        class Post
        {
        public:
            long long int id;
            std::string text;
            std::string date;

        public:
            Post(const int &id, const std::string &text, const std::string &date) : id(id), text(text), date(date){};
            Post(std::map<std::string, std::string> m_map);
            Post() = default;
            friend std::ostream &operator<<(std::ostream &os, const Post &post);
            std::string stringify() const;
            void parse(const std::string &json_str);
        };
        std::ostream &operator<<(std::ostream &os, const Post &post);

        class PostDao
        {
        private:
            sqlite3 *db;

        public:
            PostDao(sqlite3 *db) : db(db)
            {
            }
            std::pair<bool, std::string> INSERT(Post &post);
            std::pair<bool, std::string> DELETE(Post &post);
            std::pair<bool, std::string> UPDATE(Post &post);
            std::tuple<bool, std::string, std::list<Post>> SELECT(Post &post);
            std::tuple<bool, std::string, std::list<Post>> SELECT_PAGE(long long int page, int page_size);
        };
    }
}