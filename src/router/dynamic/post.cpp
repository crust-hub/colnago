#include <string>
#include <nlohmann/json.hpp>
#include "router/dynamic/post.h"
#include "server/server.h"
#include "dao/post/post-odb.hxx"
#include "dao/post/post-odb.ixx"
#include "dao/post/post.h"
#include "entity/response.h"
#include "dao/db.h"
#include "service/service.h"

using namespace colnago::entity;
using namespace colnago::db;
using namespace colnago::service;

namespace colnago
{
    namespace router
    {
        namespace PostController
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
                odb::transaction t(db::db->begin());
                auto resList = Service<Post>::getById(stoi(request->get_path_parameter("id")));
                t.commit();
                BaseResponse<Post> base_response(true, "success", *resList);
                auto resJson = base_response.stringify([](Post &p) -> string
                                                       { return p.to_json(); });
                session->close(restbed::OK, resJson, ResponseHeader::Base(ResponseHeader::JSON));
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
                    Post postObj;
                    postObj.parser(string(body.begin(), body.end()));
                    //插入
                    odb::transaction t(db::db->begin());
                    auto auto_id = Service<Post>::add(postObj, postObj.id());
                    if (auto_id > 0)
                    {
                        t.commit();
                    }
                    BaseResponse<> response(auto_id > 0);
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
                odb::transaction t(db::db->begin());
                Service<Post>::deleteById(stoll(id));
                t.commit();
                BaseResponse<> response(true);
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
                    string m_body(body.begin(), body.end());
                    Post postObj;
                    postObj.parser(string(body.begin(), body.end()));
                    // update
                    odb::transaction t(db::db->begin());
                    Service<Post>::update(postObj);
                    t.commit();
                    BaseResponse<> response(true);
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
