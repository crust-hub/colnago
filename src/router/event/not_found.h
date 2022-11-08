#pragma once
#include <memory>
#include <restbed>
#include <exception>

namespace colnago
{
    namespace router
    {
        namespace event
        {
            using namespace std;
            using namespace restbed;
            class not_found
            {
            public:
                static void event(const shared_ptr<Session> session);
            };
        }
    }
}