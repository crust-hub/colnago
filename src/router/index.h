#pragma once
#include <memory>
#include <restbed>
#include <nlohmann/json.hpp>
#include <string>
#include "request/requestTool.h"
#include "server/m_server.h"
#include "dao/user/user.h"
#include "dao/dao_api.h"

namespace colnago
{
    namespace router
    {
        namespace Index
        {
            void GET(const std::shared_ptr<restbed::Session> session);
            void DELETE(const std::shared_ptr<restbed::Session> session);
            void POST(const std::shared_ptr<restbed::Session> session);
            void PUT(const std::shared_ptr<restbed::Session> session);
        };
    }
}
