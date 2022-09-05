#include "request_tool.h"
using namespace std;
namespace colnago
{
    namespace request
    {
        size_t push_string(void *buffer, size_t size, size_t nmemb, void *stream)
        {
            string data((const char *)buffer, (size_t)size * nmemb);
            *((stringstream *)stream) << data << endl;
            return size * nmemb;
        }
        RequestTool::RequestTool()
        {
            curl = curl_easy_init();
        }

        RequestTool::~RequestTool()
        {
            curl_easy_cleanup(curl);
        }
        shared_ptr<stringstream> RequestTool::request(const char *url)
        {
            CURLcode res_code = curl_global_init(CURL_GLOBAL_ALL);
            shared_ptr<stringstream> res_back = make_shared<stringstream>();
            stringstream *res_str = res_back.get();
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, push_string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, res_str);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            res_code = curl_easy_perform(curl);
            if (res_code == CURLE_OK) //成功
            {
            }
            return res_back;
        }
    }
}