#pragma once
#include <memory>
#include <restbed>

namespace colnago
{
    namespace router
    {
        namespace PostController
        {
            void GET(const std::shared_ptr<restbed::Session> session);
            void DELETE(const std::shared_ptr<restbed::Session> session);
            void POST(const std::shared_ptr<restbed::Session> session);
            void PUT(const std::shared_ptr<restbed::Session> session);
            std::shared_ptr<restbed::Resource> resource(std::shared_ptr<restbed::Service> service);
        };
    }
}
