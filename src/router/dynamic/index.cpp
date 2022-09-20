#include <iostream>
#include <tuple>
#include <string>
#include "router/dynamic/index.h"
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "server/m_server.h"
#include "entity/response.h"
#include "dao/post/post.h"
#include "view/asset.h"

using namespace colnago::entity;
using namespace colnago::dao;
using namespace colnago::view;

namespace colnago
{
    namespace router
    {
        namespace index
        {
            using namespace std;
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                Post post(0, "", "");
                const auto request = session->get_request();
                auto page_str = request->get_query_parameter("page");
                long long int page = 1;
                if (page_str != "")
                {
                    page = stoll(page_str);
                }
                auto res_tuple = colnago::server::server.postDao->SELECT_PAGE(page, 10);
                BaseResponse<Post> base_response(std::get<0>(res_tuple), std::get<1>(res_tuple), std::get<2>(res_tuple));
                auto res = base_response.stringify([](Post &post) -> std::string
                                                   { return post.stringify(); });
                std::string response = colnago::view::render(Asset::source("index.html").c_str(), nlohmann::json::parse(res));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
            }

            std::shared_ptr<restbed::Resource> resource()
            {
                const char *postRestFul = "/";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(postRestFul);
                resource->set_method_handler("GET", GET);
                return resource;
            }
        }
    }
}