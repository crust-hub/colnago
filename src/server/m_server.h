#pragma once
#include "request/request_tool.h"
#include "router/index.h"
#include <memory>
#include "dao/dao_api.h"
#include "dao/init_db.h"
#include "dao/user/user.h"
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
            std::shared_ptr<UserDao> userDao;

        public:
            void start();
        };
        extern Server server;
    }
}
