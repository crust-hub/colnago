#include "image.h"

Image::Image(long long int id, const std::vector<char> &data, const std::string &type)
{
    id_ = id;
    data_ = data;
    type_ = type;
}

Image::Image()
{
}

const long long int Image::id() const
{
    return id_;
}

const std::vector<char> &Image::data() const
{
    return data_;
}

const std::string &Image::type() const
{
    return type_;
}
