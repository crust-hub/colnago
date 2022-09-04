#include "user.h"
#include <nlohmann/json.hpp>
#include <string>
#include "entity/response.h"
#include <nlohmann/json.hpp>
#include <string>
#include "request/request_tool.h"
#include "server/m_server.h"
#include "dao/user/user.h"
#include "dao/dao_api.h"

namespace colnago
{
    namespace router
    {
        namespace user
        {
            using json = nlohmann::json;
            using namespace std;

            /**
             * @brief GET /user/:id 检索用户
             *
             * @param session
             */
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                using namespace std;
                const auto request = session->get_request();
                colnago::dao::User user(stoi(request->get_path_parameter("id")), "", 0);
                auto res_tuple = colnago::server::server.userDao->SELECT(user);
                colnago::entity::BaseResponse<colnago::dao::User> base_response(std::get<0>(res_tuple), std::get<1>(res_tuple), std::get<2>(res_tuple));
                auto res = base_response.stringify([](colnago::dao::User &user) -> std::string
                                                   { return user.stringify(); });
                session->close(restbed::OK, res, {{"Content-Type", "application/json"}});
            }

            /**
             * @brief POST /user/:id 添加用户
             *
             * @param session
             */
            void POST(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                int content_length = stoi(request->get_header("Content-Length", "0"));
                std::pair<bool, std::string> res;
                session->fetch(content_length, [&res](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                               {
                                   colnago::dao::User userObj;
                                   userObj.parse(string(body.begin(), body.end()));
                                   res = colnago::server::server.userDao->INSERT(userObj); });
                cout << "insert end" << endl;
                colnago::entity::BaseResponse<> response(res.first, res.second);
                session->close(restbed::OK, response.stringify(), {{"Content-Type", "application/json"}});
            }

            /**
             * @brief DELETE /user/:id 删除用户
             *
             * @param session
             */
            void DELETE(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                const string id = request->get_path_parameter("id");
                colnago::dao::User delete_user(stoi(id), "", 0);
                auto res = colnago::server::server.userDao->DELETE(delete_user);
                colnago::entity::BaseResponse<> response(res.first, res.second);
                session->close(restbed::OK, response.stringify(), {{"Content-Type", "application/json"}});
            }

            /**
             * @brief PUT /user/:id 更新用户
             *
             * @param session
             */
            void PUT(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                int content_length = stoi(request->get_header("Content-Length", "0"));
                std::pair<bool, std::string> res;
                session->fetch(content_length, [&res](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                               {
                                    colnago::dao::User userObj;
                                    userObj.parse(string(body.begin(), body.end()));
                                    res = colnago::server::server.userDao->UPDATE(userObj); });
                colnago::entity::BaseResponse<> response(res.first, res.second);
                session->close(restbed::OK, response.stringify(), {{"Content-Type", "application/json"}});
            }

            std::shared_ptr<restbed::Resource> resource()
            {
                //路由信息
                const char *userRestFul = "/user/{id: [0-9]+}";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(userRestFul);
                resource->set_method_handler("GET", GET);
                resource->set_method_handler("PUT", PUT);
                resource->set_method_handler("DELETE", DELETE);
                resource->set_method_handler("POST", POST);
                return resource;
            }
        }
    }
}
