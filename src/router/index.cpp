#include "index.h"
#include <iostream>
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "server/m_server.h"
#include "entity/response.h"
#include "dao/user/user.h"
#include <tuple>
#include <string>

namespace colnago
{
    namespace router
    {
        namespace index
        {
            using namespace std;
            void GET(const std::shared_ptr<restbed::Session> session)
            {
                colnago::dao::User user(0, "", 0);
                auto res_tuple = colnago::server::server.userDao->SELECT(user);
                colnago::entity::BaseResponse<colnago::dao::User> base_response(std::get<0>(res_tuple), std::get<1>(res_tuple), std::get<2>(res_tuple));
                auto res = base_response.stringify([](colnago::dao::User &user) -> std::string
                                                   { return user.stringify(); });
                std::string response = colnago::view::render("../src/view/asset/index.html", nlohmann::json::parse(res));
                session->close(restbed::OK, response, {{"Content-Type", "text/html"}});
            }

            std::shared_ptr<restbed::Resource> resource()
            {
                const char *userRestFul = "/";
                auto resource = make_shared<restbed::Resource>();
                resource->set_path(userRestFul);
                resource->set_method_handler("GET", GET);
                return resource;
            }
        }
    }
}