#include <tuple>
#include <string>
#include <nlohmann/json.hpp>
#include "router/dynamic/detail.h"
#include "view/view.h"
#include "server/server.h"
#include "entity/response.h"
#include "dao/post/post-odb.hxx"
#include "dao/post/post-odb.ixx"
#include "dao/post/post.h"
#include "view/asset.h"
#include "dao/db.h"
#include "service/service.h"

using namespace colnago::entity;
using namespace colnago::db;
using namespace colnago::view;
using namespace colnago::service;

using namespace std;

namespace colnago
{
    namespace router
    {
        namespace DetailController
        {
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                auto id_str = request->get_path_parameter("id");
                auto id = stoll(id_str);
                odb::transaction t(db::db->begin());
                auto resList = Service<Post>::getById(id);
                t.commit();
                BaseResponse<Post> base_response(true, "success", *resList);
                auto func = [](Post &item) -> std::string
                {
                    return item.to_json();
                };
                auto resStr = base_response.stringify(func);
                std::string response = colnago::view::render(Asset::source("detail.html").c_str(), nlohmann::json::parse(resStr));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
            }

            std::shared_ptr<restbed::Resource> resource(std::shared_ptr<restbed::Service> service)
            {
                const char *postRestFul = "/detail/{id: [0-9]+}";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(postRestFul);
                resource->set_method_handler("GET", GET);
                service->publish(resource);
                return resource;
            }
        }
    }

}
