#pragma once
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <memory>
namespace colnago
{
    namespace utils
    {
        namespace request
        {
            using namespace std;
            class RequestToolAPI
            {
            public:
                virtual shared_ptr<stringstream> request(const char *url) = 0;
            };

            class RequestTool : public RequestToolAPI
            {
            private:
                CURL *curl;

            public:
                RequestTool();
                ~RequestTool();
                shared_ptr<stringstream> request(const char *url);
            };
        }
    }
}