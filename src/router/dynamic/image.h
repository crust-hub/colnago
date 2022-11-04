#pragma once
#include <memory>
#include <restbed>

namespace colnago
{
    namespace router
    {
        namespace ImageController
        {
            void POST(const std::shared_ptr<restbed::Session> session);
            void GET(const std::shared_ptr<restbed::Session> session);
            void GET_LIST(const std::shared_ptr<restbed::Session> session);
            void IMAGES_PAGE(const std::shared_ptr<restbed::Session> session);
            std::shared_ptr<restbed::Resource> resource(std::shared_ptr<restbed::Service> service);
        };
    }
}
