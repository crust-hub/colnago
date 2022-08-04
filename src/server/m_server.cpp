#include "m_server.h"
namespace colnago
{
    namespace server
    {
        void Server::start()
        {
            //数据库配置
            db = ColnagoDB::create("colnago.db");
            userDao = std::make_shared<UserDao>(db->db);

            // 创建服务
            restbed::Service service;

            // server配置
            auto settings = make_shared<restbed::Settings>();
            settings->set_port(20003);
            // settings->set_default_header("Connection", "close"); //默认响应头

            std::cout << "start server on 20003 port" << std::endl;

            //路由信息
            const char *userRestFul = "/user/{id: [0-9]*}";

            auto resource = make_shared<restbed::Resource>();
            resource->set_path(userRestFul);
            resource->set_method_handler("GET", colnago::router::Index::GET);
            resource->set_method_handler("PUT", colnago::router::Index::PUT);
            resource->set_method_handler("DELETE", colnago::router::Index::DELETE);
            resource->set_method_handler("POST", colnago::router::Index::POST);
            service.publish(resource); //发布路由

            service.start(settings); //开启服务
        }
        Server server;
    }
}