#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "router/dynamic/image.h"
#include "entity/response.h"

using namespace colnago::router;
using namespace std;
using namespace colnago::entity;

/**
 * @brief POST /image
 *
 * @param session
 */
void image::POST(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    long long int content_length = stoll(request->get_header("Content-Length", "0"));
    string content_type = request->get_header("Content-Type");
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

    //算法待优化，不应该使用内存消耗，优化为行数记录策略
    auto handler = [slice_line, end_line, content_type](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
    {
        istringstream body_stream(string(body.begin(), body.end()));
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

        int headers_count = 0, bodys_count = 0;
        //解析内容每个part
        for (size_t i = 0; i < parts.size(); i++)
        {
            stringstream part(parts[i]);
            //解析头部部分
            string line;
            // vector<string> header_lines;
            while (part.good())
            {
                getline(part, line);
                if (line == "\r")
                {
                    break;
                }
                // header_lines.push_back(line);
            }
            headers_count++;

            // stringstream body;
            while (part.good()) //收录body部分
            {
                getline(part, line);
                if (part.good())
                {
                    // body << line << "\n";
                }
                else
                {
                    // body << line;
                }
            }
            bodys_count++;
        }

        cout << headers_count << " " << bodys_count << endl;

        session->close(restbed::OK, "post success", {{"Content-Type", "text/text"}});
    };
    session->fetch(content_length, handler);
}

std::shared_ptr<restbed::Resource> image::resource()
{
    const char *postRestFul = "/image";
    auto resource = make_shared<restbed::Resource>();
    resource->set_path(postRestFul);
    resource->set_method_handler("POST", POST);
    return resource;
}