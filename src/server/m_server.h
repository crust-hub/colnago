#pragma once
#include <memory>
#include "dao/dao_api.h"
#include "dao/init_db/init_db.h"
#include "dao/post/post.h"
#include "router/router.h"

namespace colnago
{
    namespace server
    {
        using namespace std;
        using namespace colnago::dao;
        class Server
        {
        public:
            std::shared_ptr<DaoAPI> db;
            std::shared_ptr<PostDao> postDao;

        protected:
            void router_publish();
            std::shared_ptr<restbed::Service> service;

        public:
            void start();
        };
        extern Server server;
    }
}
