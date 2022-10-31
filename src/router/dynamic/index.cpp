#include <iostream>
#include <tuple>
#include <string>
#include <list>
#include "router/dynamic/index.h"
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "server/server.h"
#include "entity/response.h"
#include "dao/post/post-odb.hxx"
#include "dao/post/post-odb.ixx"
#include "dao/post/post.h"
#include "view/asset.h"
#include "dao/db.h"

using namespace colnago::entity;
using namespace colnago::view;
using namespace colnago::db;

namespace colnago
{
    namespace router
    {
        namespace IndexController
        {
            using namespace std;

            /**
             * @brief 主页信息
             *
             * @param session
             */
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
                //查找
                std::list<Post> res_list;
                odb::transaction t(db::db->begin());
                odb::result<Post> all(db::db->query<Post>());
                for (auto &item : all)
                {
                    res_list.push_back(item);
                }
                t.commit();
                BaseResponse<Post> base_response(true, "检索成功", res_list);
                auto res = base_response.stringify([](Post &post) -> std::string
                                                   { return post.to_json(); });
                std::string response = colnago::view::render(Asset::source("index.html").c_str(), nlohmann::json::parse(res));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
                return;
            }

            std::shared_ptr<restbed::Resource> resource(std::shared_ptr<restbed::Service> service)
            {
                const char *postRestFul = "/";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(postRestFul);
                resource->set_method_handler("GET", GET);
                service->publish(resource);
                return resource;
            }
        }
    }
}
