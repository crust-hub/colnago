#pragma once

#include <string>
#include <odb/core.hxx>

#pragma db object table("post")
class Post
{
public:
    Post(long long int id, const std::string &text, const std::string &date);
    Post();
    const long long int id() const;
    const std::string &text() const;
    const std::string &date() const;
    std::string to_json();
    void parser(const std::string &jsonStr);

private:
    friend class odb::access;
#pragma db id auto
    long long int id_;
    std::string text_;
    std::string date_;
};
// odb -d mysql --generate-query --generate-schema post.h