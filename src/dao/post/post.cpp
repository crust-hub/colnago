#include <nlohmann/json.hpp>
#include "post.h"

using json = nlohmann::json;

Post::Post(long long int id, const std::string &text, const std::string &date)
{
    id_ = id;
    text_ = text;
    date_ = date;
}

Post::Post()
{
}

const long long int Post::id() const
{
    return id_;
}

const std::string &Post::text() const
{
    return text_;
}

const std::string &Post::date() const
{
    return date_;
}

std::string Post::to_json()
{
    // escap char
    json res = {{"id", id_}, {"date", date_}, {"text", text_}};
    // std::cout<<j_object.dump()<<std::endl;
    return res.dump();
}

void Post::parser(const std::string &jsonStr)
{
    json res = json::parse(jsonStr);
    id_ = res["id"];
    text_ = res["text"];
    date_ = res["date"];
}
