#pragma once
#include <memory>
#include "router/router.h"

namespace colnago
{
    namespace server
    {
        using namespace std;
        class Server
        {
        protected:
            void router_publish();
            std::shared_ptr<restbed::Service> service;

        public:
            void start();
        };
        extern Server server;
    }
}
