#include <tuple>
#include <string>

#include "router/dynamic/detail.h"
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "server/m_server.h"
#include "entity/response.h"
#include "dao/post/post.h"
#include "view/asset.h"

using namespace colnago::entity;
using namespace colnago::dao;
using namespace colnago::view;
using namespace std;

namespace colnago
{
    namespace router
    {
        namespace detail
        {
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                auto id_str = request->get_path_parameter("id");
                auto id = stoll(id_str);
                Post post(id, "", "");
                auto res_tuple = colnago::server::server.postDao->SELECT(post);
                BaseResponse<Post> base_response(std::get<0>(res_tuple), std::get<1>(res_tuple), std::get<2>(res_tuple));
                auto res = base_response.stringify([](Post &post) -> std::string
                                                   { return post.stringify(); });
                std::string response = colnago::view::render(Asset::source("detail.html").c_str(), nlohmann::json::parse(res));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
            }

            std::shared_ptr<restbed::Resource> resource()
            {
                const char *postRestFul = "/detail/{id: [0-9]+}";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(postRestFul);
                resource->set_method_handler("GET", GET);
                return resource;
            }
        }
    }
}