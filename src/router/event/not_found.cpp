#include "router/event/not_found.h"
#include <limits.h>
#include <sys/stat.h>
#include <memory>
#include <vector>
#include <fstream>
#include "view/asset.h"
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "entity/response.h"
#include "utils/util.h"
#include "utils/mime_types.h"

using namespace colnago::router::event;
using namespace colnago::entity;
using namespace colnago::view;
using namespace colnago::utils;

void not_found::event(const shared_ptr<Session> session)
{
    auto request = session->get_request();
    const auto path = request->get_path();
    //在resources目录下寻找是否有这个文件
    char buf[PATH_MAX];
    static const string preffix = string("./resources/");
    const string str_path = preffix + path;
    realpath(str_path.c_str(), buf);
    auto last_path = string(buf);
    //真实允许的路径
    realpath(preffix.c_str(), buf);
    static const string permit = string(buf);
    if (util::string_start_with(last_path, permit) && permit != last_path)
    {
        FILE *fp = fopen(last_path.c_str(), "r");
        if (fp)
        {
            cout << "存在此文件" << endl;
            struct stat statbuf;
            stat(last_path.c_str(), &statbuf);
            char buffer[1024];
            restbed::Bytes chunk;
            int len = 0;
            while ((len = fread(buffer, 1024, 1, fp)) > 0)
            {
                chunk.clear();
                for (int i = 0; i < len; i++)
                {
                    chunk.push_back(buffer[i]);
                }
                // session->yield(200, chunk);
                session->yield(restbed::OK, chunk, {{"Transfer-Encoding", "chunked"}}, [&](shared_ptr<Session> session) -> void
                               { session->close(); });
                break;
            }
            fclose(fp);
            // session->close();
        }
    }
    else
    {
        if (session->is_closed())
        {
            return;
        }
        static std::shared_ptr<std::string> html;
        if (html == nullptr)
        {
            html = std::make_shared<std::string>();
            nlohmann::json data;
            *html = colnago::view::render(Asset::source("404.html").c_str(), data);
        }
        session->close(restbed::NOT_FOUND, *html, ResponseHeader::Base(ResponseHeader::HTML));
    }
}