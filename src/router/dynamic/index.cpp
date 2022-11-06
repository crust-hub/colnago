#include <iostream>
#include <tuple>
#include <string>
#include <list>
#include <nlohmann/json.hpp>
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
#include "service/service.h"

using namespace colnago::entity;
using namespace colnago::view;
using namespace colnago::db;
using namespace colnago::service;

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
                std::string response = colnago::view::render(Asset::source("index.html").c_str(), nlohmann::json::parse("{}"));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
                return;
            }

            /**
             * @brief 帖子页面 /blogs
             *
             * @param session
             */
            void BLOGS_PAGE(const std::shared_ptr<restbed::Session> session)
            {
                const auto request = session->get_request();
                auto page_str = request->get_query_parameter("page");
                long long int page = 1;
                if (page_str != "")
                {
                    page = stoll(page_str);
                }
                odb::transaction t(db::db->begin());
                //排序最新的帖子为第一页
                auto res_list = Service<Post>::page(odb::query<Post>::id.is_not_null() + "ORDER BY" + odb::query<Post>::id + "DESC", 10, page);
                t.commit();
                BaseResponse<Post> base_response(true, "检索成功", *res_list);
                auto res = base_response.stringify([](Post &post) -> std::string
                                                   { return post.to_json(); });
                std::string response = colnago::view::render(Asset::source("blogs.html").c_str(), nlohmann::json::parse(res));
                session->close(restbed::OK, response, ResponseHeader::Base(ResponseHeader::HTML));
                return;
            }

            std::shared_ptr<restbed::Resource> resource(std::shared_ptr<restbed::Service> service)
            {
                auto resource = make_shared<restbed::Resource>();
                resource->set_path("/");
                resource->set_method_handler("GET", GET);

                auto resource_blogs = make_shared<restbed::Resource>();
                resource_blogs->set_path("/blogs");
                resource_blogs->set_method_handler("GET", BLOGS_PAGE);

                service->publish(resource);
                service->publish(resource_blogs);

                return resource;
            }
        }
    }
}
