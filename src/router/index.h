#pragma once
#include <memory>
#include <restbed>
#include <nlohmann/json.hpp>
#include <string>
#include "../request/requestTool.h"

namespace courscpp
{
    namespace router
    {
        using namespace std;
        class index
        {
        public:
            static void get(const shared_ptr<restbed::Session> session)
            {
                shared_ptr<courscpp::request::RequestToolAPI> m_request = make_shared<courscpp::request::RequestTool>();
                const auto request = session->get_request();
                const string name = request->get_path_parameter("name");
                const string protocol = request->get_path_parameter("protocol");
                const string path = protocol + "://" + name;
                auto res = m_request->request(path.c_str());
                auto j = R"(
                {
                    "name":"gaowanlu",
                    "age":20,
                    "Content-Length":"",
                    "Accept":""
                }
                )"_json;
                j["name"] = "hi";
                // request->get_header(string("Content-Length"));
                j["Content-Length"] = res->str().length();
                j["Accept"] = request->get_header(string("Accept"));
                // j.dump()
                session->close(restbed::OK, res->str(), {{"Content-Type", "text/html"}, {"Content-Length", to_string(res->str().length()).c_str()}});
            }
        };
    }
}
