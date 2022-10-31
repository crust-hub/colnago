#include <iostream>
#include <chrono>
#include "server/server.h"
#include "router/other/static_router.h"

namespace colnago
{
    namespace server
    {
        void Server::start()
        {
            // 创建服务
            service = std::make_shared<restbed::Service>();
            // server配置
            auto settings = make_shared<restbed::Settings>();
            settings->set_port(20003);
            settings->set_worker_limit(50); //最大线程
            settings->set_connection_timeout(std::chrono::seconds(10));//10s
            //settings->set_default_header("Connection", "close"); //默认响应头

            std::cout << "start server on 20003 port" << std::endl;

            //发布路由
            router_publish();

            //开启服务
            service->start(settings);
        }

        void Server::router_publish()
        {
            colnago::router::PostController::resource(service);
            colnago::router::IndexController::resource(service);
            colnago::router::DetailController::resource(service);
            colnago::router::ImageController::resource(service);
            service->set_not_found_handler(colnago::router::other::static_router::default_event);
        }

        Server server;
    }
}
