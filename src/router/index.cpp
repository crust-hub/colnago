#include "index.h"
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

namespace colnago
{
    namespace router
    {
        namespace Index
        {
            using namespace std;
            //检索用户
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                using namespace std;
                shared_ptr<colnago::request::RequestToolAPI> m_request = make_shared<colnago::request::RequestTool>();
                const auto request = session->get_request();
                const string id = request->get_path_parameter("id");

                colnago::dao::User user(stoi(id), "", 0); //查询所有用户
                auto res_tuple = colnago::server::server.userDao->SELECT(user);

                auto response_content = R"({
                    "result":true,
                    "message":"",
                    "list":[]
                })"_json;

                response_content["result"] = std::get<0>(res_tuple);
                response_content["message"] = std::get<1>(res_tuple);
                auto users = std::get<2>(res_tuple);

                json user_array = json::array();
                auto user_template = R"(
                    {
                        "id":0,
                        "num":20,
                        "name":""
                    })"_json;
                for (auto item : users)
                {
                    user_template["id"] = item.id;
                    user_template["num"] = item.num;
                    user_template["name"] = item.name;
                    user_array.push_back(user_template);
                }
                response_content["list"] = user_array;

                session->close(restbed::OK, response_content.dump(), {{"Content-Type", "application/json"}});
            }

            //添加用户
            void POST(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                int content_length = stoi(request->get_header("Content-Length", "0"));
                std::pair<bool, std::string> res;
                session->fetch(content_length, [&res](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                               {
                                   //获取请求体的字符串形式
                                   string user_json(body.begin(), body.end());
                                   //转为json对象
                                   json user = json::parse(user_json);
                                   colnago::dao::User userObj(user["id"], user["name"], user["num"]);
                                   res = colnago::server::server.userDao->INSERT(userObj); });
                auto res_template = R"(
                    {
                        "result":false,
                        "message":""
                    })"_json;
                res_template["result"] = res.first;
                res_template["message"] = res.second;
                session->close(restbed::OK, res_template.dump(), {{"Content-Type", "application/json"}});
            }

            //删除用户
            void DELETE(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                const string id = request->get_path_parameter("id");
                colnago::dao::User delete_user(stoi(id), "", 0);
                auto res = colnago::server::server.userDao->DELETE(delete_user);
                auto res_template = R"(
                    {
                        "result":false,
                        "message":""
                    })"_json;
                res_template["result"] = res.first;
                res_template["message"] = res.second;
                session->close(restbed::OK, res_template.dump(), {{"Content-Type", "application/json"}});
            }

            //更新用户
            void PUT(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                int content_length = stoi(request->get_header("Content-Length", "0"));
                std::pair<bool, std::string> res;
                session->fetch(content_length, [&res](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                               {
                                   //获取请求体的字符串形式
                                   string user_json(body.begin(), body.end());
                                   //转为json对象
                                   json user = json::parse(user_json);
                                   colnago::dao::User userObj(user["id"], user["name"], user["num"]);
                                   res = colnago::server::server.userDao->UPDATE(userObj); });
                auto res_template = R"(
                    {
                        "result":false,
                        "message":""
                    })"_json;
                res_template["result"] = res.first;
                res_template["message"] = res.second;
                session->close(restbed::OK, res_template.dump(), {{"Content-Type", "application/json"}});
            }
        }
    }
}