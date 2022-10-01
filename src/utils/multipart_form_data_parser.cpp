#include <cstring>
#include <utility>

#include "utils/multipart_form_data_parser.h"

using namespace std;
using namespace colnago::utils;

std::pair<vector<std::map<string, string>>, std::vector<std::string>> multipart_form_data_parser::parser(std::istringstream &body_stream, const string &slice_line, const string &end_line)
{
    string line;
    vector<string> parts;
    stringstream temp;
    bool skiped = false;

    while (body_stream.good())
    {
        getline(body_stream, line);
        if (line == slice_line || line == end_line)
        {
            if (skiped == false)
            {
                skiped = true;
                continue;
            }
            parts.push_back(temp.str());
            temp.clear();
            temp.str("");
        }
        else
        {
            line = line + "\n";
            temp << line;
        }
    }

    // free origin data
    body_stream.clear();
    body_stream.str("");

    vector<vector<string>> headers;
    vector<string> bodys;

    //解析内容每个part
    for (size_t i = 0; i < parts.size(); i++)
    {
        stringstream part(parts[i]);
        //解析头部部分
        string line;
        vector<string> header_lines;
        while (part.good())
        {
            getline(part, line);
            if (line == "\r")
            {
                break;
            }
            header_lines.push_back(line);
        }
        headers.push_back(header_lines);

        stringstream body;
        while (part.good()) //收录body部分
        {
            getline(part, line);
            if (part.good())
            {
                body << line << "\n";
            }
            else
            {
                body << line;
            }
        }

        bodys.push_back(body.str());

        part.clear();
        part.str("");
        body.clear();
        body.str("");
    }

    vector<map<string, string>> header_maps;

    for (auto &item : headers)
    {
        map<string, string> header_map;
        for (auto &line : item)
        {
            //找到第一个:
            auto split_index = line.find(": ");
            header_map.insert({line.substr(0, split_index), line.substr(split_index + 2)});
        }
        header_maps.push_back(header_map);
    }

    return make_pair(header_maps, bodys);
}

std::pair<std::string, std::string> multipart_form_data_parser::boundary(std::string content_type)
{
    size_t i = content_type.find("multipart/form-data;");

    if (i == 0)
    {
        content_type.erase(i, strlen("multipart/form-data;"));
    }

    i = content_type.find_first_not_of(' '); //去空格
    if (i != string::npos)
    {
        content_type.erase(0, i);
    }

    i = content_type.find_first_of("boundary=");
    if (i == 0)
    {
        content_type.erase(i, strlen("boundary="));
    }

    string slice_line = string("--") + content_type + "\r";
    string end_line = string("--") + content_type + "--\r";
    return make_pair(slice_line, end_line);
}