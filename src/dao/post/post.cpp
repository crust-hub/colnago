#include <sstream>
#include <cstdio>
#include <map>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "inja/inja.hpp"
#include "dao/post/post.h"
#include "utils/sql_easy_runner.h"
#include "dao/dao_api.h"

using namespace std;

namespace colnago
{
    namespace dao
    {
        std::string Post::stringify() const
        {
            using namespace colnago::utils::sql_easy_runner;
            using json = nlohmann::json;
            // escap char
            json j_object = {{"id", id}, {"date", date}, {"text", text}};
            // std::cout<<j_object.dump()<<std::endl;
            return j_object.dump();
        }

        std::ostream &operator<<(std::ostream &os, const Post &post)
        {
            os << post.stringify();
            return os;
        }

        void Post::parse(const std::string &json_str)
        {
            using json = nlohmann::json;
            using namespace colnago::utils::sql_easy_runner;
            try
            {
                json json_obj = json::parse(json_str);
                id = json_obj["id"];
                text = escap_char(json_obj["text"]);
                date = json_obj["date"];
            }
            catch (...)
            {
            }
        }

        std::pair<bool, std::string> PostDao::INSERT(Post &post)
        {
            // escape character
            nlohmann::json data = nlohmann::json::parse(post.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(INSERT INTO POST (TEXT,DATE) VALUES ('{{ text }}',strftime('%Y/%m/%d %H:%M:%S','now') );)", data);
            if ("" == data["text"])
            {
                return std::pair<bool, std::string>(false, "text cannot none");
            }
            auto res = colnago::utils::sql_easy_runner::esay_run(sql, db);
            return res;
        }

        std::pair<bool, std::string> PostDao::DELETE(Post &post)
        {
            nlohmann::json data = nlohmann::json::parse(post.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(DELETE FROM POST WHERE ID = {{ id }})", data);
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::pair<bool, std::string> PostDao::UPDATE(Post &post)
        {
            nlohmann::json data = nlohmann::json::parse(post.stringify());
            std::stringstream sql;
            inja::render_to(sql, R"(UPDATE POST SET TEXT = '{{ text }}',DATE = '{{ date }}' WHERE ID = {{ id }})", data);
            return colnago::utils::sql_easy_runner::esay_run(sql, db);
        }

        std::tuple<bool, std::string, std::list<Post>> PostDao::SELECT(Post &post)
        {
            using namespace colnago::utils::sql_easy_runner;
            nlohmann::json data = nlohmann::json::parse(post.stringify());
            std::stringstream sql;
            if (post.id == 0)
                inja::render_to(sql, R"(SELECT * FROM POST ORDER BY ID;)", data);
            else
                inja::render_to(sql, R"(SELECT * FROM POST WHERE ID = {{ id }} ORDER BY ID;)", data);
            std::list<Post> posts;
            //回调函数
            auto callback = [](void *data, int argc, char **argv, char **azColName) mutable -> int
            {
                std::list<Post> *posts_ptr = (std::list<Post> *)data;
                std::list<Post> &posts = *posts_ptr;
                std::map<std::string, std::string> m_map;
                for (int i = 0; i < argc; i++)
                {
                    Post new_post;
                    std::string key(azColName[i]);
                    std::string value(argv[i]);
                    m_map[key] = value;
                }
                Post m_post;
                m_post.id = stoi(m_map["ID"]);
                std::string text = m_map["TEXT"];
                m_post.text = parse_escap_char(text);
                m_post.date = m_map["DATE"];
                posts.push_back(m_post);
                return 0;
            };
            std::list<Post> *posts_ptr = &posts;
            auto res_pair = colnago::utils::sql_easy_runner::callback_run(sql, db, callback, (void *)posts_ptr);
            std::tuple<bool, std::string, std::list<Post>> res = std::make_tuple(res_pair.first, res_pair.second, posts);
            return res;
        }
    }
}
