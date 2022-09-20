#include "server/m_server.h"
#include "router/other/static_router.h"

namespace colnago
{
    namespace server
    {
        void Server::start()
        {
            //数据库配置
            db = ColnagoDB::create("/disk/colnago.db");
            // db = ColnagoDB::create("./colnago.db");
            postDao = std::make_shared<PostDao>(db->db);

            // 创建服务
            service = std::make_shared<restbed::Service>();

            // server配置
            auto settings = make_shared<restbed::Settings>();
            settings->set_port(20003);
            settings->set_worker_limit(50); //最大线程
            // settings->set_default_header("Connection", "close"); //默认响应头

            std::cout << "start server on 20003 port" << std::endl;

            //发布路由
            router_publish();

            //开启服务
            service->start(settings);
        }

        void Server::router_publish()
        {
            service->publish(colnago::router::post::resource());
            service->publish(colnago::router::index::resource());
            service->publish(colnago::router::detail::resource());
            service->set_not_found_handler(colnago::router::other::static_router::default_event);
        }

        Server server;
    }
}
