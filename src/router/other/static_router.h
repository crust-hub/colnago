#pragma once
#include <memory>
#include <restbed>
#include <exception>

namespace colnago
{
    namespace router
    {
        namespace other
        {
            using namespace std;
            using namespace restbed;
            class static_router
            {
            public:
                static void default_event(const shared_ptr<Session> session);
            };
        }
    }
}