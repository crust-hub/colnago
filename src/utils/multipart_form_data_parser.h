#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

namespace colnago
{
    namespace utils
    {
        class multipart_form_data_parser
        {
        public:
            static std::pair<std::vector<std::map<std::string, std::string>>, std::vector<std::string>> parser(std::istringstream &body_stream, const std::string &slice_line, const std::string &end_line);
            static std::pair<std::string, std::string> boundary(std::string content_type);
        };
    }
}