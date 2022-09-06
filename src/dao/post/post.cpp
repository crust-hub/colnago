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

using namespace colnago::dao;
using namespace std;
using namespace colnago::utils::sql_easy_runner;
using json = nlohmann::json;

Post::Post(std::map<std::string, std::string> m_map)
{
    id = stoll(m_map["ID"]);
    text = parse_escap_char(m_map["TEXT"]);
    date = m_map["DATE"];
}

std::string Post::stringify() const
{
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
    json data = json::parse(post.stringify());
    std::stringstream sql;
    inja::render_to(sql, R"(INSERT INTO POST (TEXT,DATE) VALUES ('{{ text }}',strftime('%Y/%m/%d %H:%M:%S','now') );)", data);
    if ("" == data["text"])
    {
        return std::pair<bool, std::string>(false, "text cannot none");
    }
    std::pair<bool, std::string> res = esay_run(sql, db);
    return res;
}

std::pair<bool, std::string> PostDao::DELETE(Post &post)
{
    json data = json::parse(post.stringify());
    std::stringstream sql;
    inja::render_to(sql, R"(DELETE FROM POST WHERE ID = {{ id }})", data);
    return esay_run(sql, db);
}

std::pair<bool, std::string> PostDao::UPDATE(Post &post)
{
    json data = json::parse(post.stringify());
    std::stringstream sql;
    inja::render_to(sql, R"(UPDATE POST SET TEXT = '{{ text }}',DATE = '{{ date }}' WHERE ID = {{ id }})", data);
    return esay_run(sql, db);
}

std::tuple<bool, std::string, std::list<Post>> PostDao::SELECT(Post &post)
{
    json data = json::parse(post.stringify());
    std::stringstream sql;
    if (post.id == 0)
        inja::render_to(sql, R"(SELECT * FROM POST ORDER BY ID;)", data);
    else
        inja::render_to(sql, R"(SELECT * FROM POST WHERE ID = {{ id }} ORDER BY ID;)", data);
    std::tuple<bool, std::string, std::list<Post>> res = CallbackTolist<Post>::f(sql, db);
    return res;
}

std::tuple<bool, std::string, std::list<Post>> PostDao::SELECT_PAGE(long long int page, int page_size)
{
    long long int start = (page - 1) * page_size;
    if (start < 0)
        start = 0;
    json data = {{"page_size", page_size}, {"start", start}};
    std::stringstream sql;
    inja::render_to(sql, R"(SELECT * FROM POST ORDER BY ID DESC limit {{start}} ,{{ page_size }};)", data);
    std::tuple<bool, std::string, std::list<Post>> res = CallbackTolist<Post>::f(sql, db);
    return res;
}
