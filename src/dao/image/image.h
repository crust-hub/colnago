#pragma once

#include <string>
#include <vector>
#include <odb/core.hxx>

typedef std::vector<char> buffer;

#pragma db value(buffer) type("MEDIUMBLOB")
#pragma db object table("image")
class Image
{
public:
    Image(long long int id, const std::vector<char> &data, const std::string &type);
    Image();
    const long long int id() const;
    const buffer &data() const;
    const std::string &type() const;

private:
    friend class odb::access;
#pragma db id auto
    long long int id_;
    buffer data_;
    std::string type_;
};

// odb -d mysql --generate-query --generate-schema image.h
