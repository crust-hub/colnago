#pragma once
#include "request/requestTool.h"
#include "router/index.h"
namespace courscpp
{
    namespace server
    {
        class m_server
        {
        public:
            void start()
            {
                using namespace std;
                //路由信息
                auto resource = make_shared<restbed::Resource>();
                resource->set_path("/resource/{protocol: http|https}/{name: .*}");
                resource->set_method_handler("GET", courscpp::router::index::get);

                // server配置
                auto settings = make_shared<restbed::Settings>();
                settings->set_port(20001);
                // settings->set_default_header("Connection", "close"); //默认响应头

                // create server
                restbed::Service service;
                service.publish(resource); //发布路由
                service.start(settings);   //开启服务
            }
        };
    }
}