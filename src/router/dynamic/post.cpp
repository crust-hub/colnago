#include <string>
#include <nlohmann/json.hpp>
#include "router/dynamic/post.h"
#include "server/m_server.h"
#include "dao/post/post.h"
#include "dao/dao_api.h"
#include "entity/response.h"

using namespace colnago::entity;
using namespace colnago::dao;

namespace colnago
{
    namespace router
    {
        namespace post
        {
            using json = nlohmann::json;
            using namespace std;

            /**
             * @brief GET /post/:id
             *
             * @param session
             */
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                using namespace std;
                const auto request = session->get_request();
                Post post(stoi(request->get_path_parameter("id")), "", "");
                auto res_tuple = colnago::server::server.postDao->SELECT(post);
                BaseResponse<colnago::dao::Post> base_response(res_tuple);

                auto func = [](colnago::dao::Post &item) -> std::string
                {
                    return item.stringify();
                };

                auto res = base_response.stringify(func);
                session->close(restbed::OK, res, ResponseHeader::Base(ResponseHeader::JSON));
            }

            /**
             * @brief POST /post/:id
             *
             * @param session
             */
            void POST(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                long long int content_length = stoll(request->get_header("Content-Length", "0"));
                auto handler = [](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                {
                    string m_body(body.begin(), body.end());
                    colnago::dao::Post postObj;
                    postObj.parse(string(body.begin(), body.end()));
                    auto res = colnago::server::server.postDao->INSERT(postObj);
                    BaseResponse<> response(res);
                    session->close(restbed::OK, response.stringify(), ResponseHeader::Base(ResponseHeader::JSON));
                };
                session->fetch(content_length, handler);
            }

            /**
             * @brief DELETE /post/:id
             *
             * @param session
             */
            void DELETE(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                const string id = request->get_path_parameter("id");
                colnago::dao::Post delete_post(stoi(id), "", "");
                auto res = colnago::server::server.postDao->DELETE(delete_post);
                BaseResponse<> response(res);
                session->close(restbed::OK, response.stringify(), ResponseHeader::Base(ResponseHeader::JSON));
            }

            /**
             * @brief PUT /post/:id
             *
             * @param session
             */
            void PUT(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                int content_length = stoi(request->get_header("Content-Length", "0"));

                auto handler = [](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
                {
                    colnago::dao::Post postObj;
                    postObj.parse(string(body.begin(), body.end()));
                    auto res = colnago::server::server.postDao->UPDATE(postObj);
                    BaseResponse<> response(res);
                    session->close(restbed::OK, response.stringify(), ResponseHeader::Base(ResponseHeader::JSON));
                };

                session->fetch(content_length, handler);
            }

            std::shared_ptr<restbed::Resource> resource()
            {
                //路由信息
                const char *postRestFul = "/post/{id: [0-9]+}";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(postRestFul);
                resource->set_method_handler("GET", GET);
                resource->set_method_handler("PUT", PUT);
                resource->set_method_handler("DELETE", DELETE);
                resource->set_method_handler("POST", POST);
                return resource;
            }
        }
    }
}
