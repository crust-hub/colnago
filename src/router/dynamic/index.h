#pragma once
#include <memory>
#include <restbed>
namespace colnago
{
    namespace router
    {
        namespace IndexController
        {
            void GET(const std::shared_ptr<restbed::Session> session);
            std::shared_ptr<restbed::Resource> resource();
        }
    }
}