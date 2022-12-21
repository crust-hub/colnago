#include "router/event/not_found.h"
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>
#include <vector>
#include <fstream>
#include <unistd.h>
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

static void chunk_sender(const shared_ptr<Session> session, int fp)
{
    char buffer[512];
    ssize_t size = read(fp, buffer, 512);
    if (size == -1)
    {
        return;
    }
    string str_size;
    {
        stringstream s;
        s << hex;
        s << size;
        str_size = s.str();
    }
    restbed::Bytes bytes;
    for (char &item : str_size)
    {
        bytes.push_back(item);
    }
    bytes.push_back('\r');
    bytes.push_back('\n');
    for (int i = 0; i < size; i++)
    {
        bytes.push_back(buffer[i]);
    }
    bytes.push_back('\r');
    bytes.push_back('\n');
    if (size > 0)
        session->yield(bytes, [&](const shared_ptr<Session> session) -> void
                       { chunk_sender(session, fp); });
    else
    {
        session->close("0\r\n\r\n");
        close(fp);
        //printf("over\n");
    }
}

static void file_sender(const shared_ptr<Session> session, int fp)
{
    char buffer[512];
    ssize_t size = read(fp, buffer, 512);
    if (size == -1)
    {
        return;
    }
    string str_size;
    {
        stringstream s;
        s << hex;
        s << size;
        str_size = s.str();
    }
    restbed::Bytes bytes;
    for (char &item : str_size)
    {
        bytes.push_back(item);
    }
    bytes.push_back('\r');
    bytes.push_back('\n');
    for (int i = 0; i < size; i++)
    {
        bytes.push_back(buffer[i]);
    }
    bytes.push_back('\r');
    bytes.push_back('\n');
    //, {"Content-Disposition", "attchment"}
    session->yield(OK, bytes, {{"Transfer-Encoding", "chunked"}, {"Content-Disposition", "attchment"}}, [&](const shared_ptr<Session> session) -> void
                   { chunk_sender(session, fp); });
}

void not_found::event(const shared_ptr<Session> session)
{
    auto request = session->get_request();
    const string path = request->get_path();
    // 检查是否有..,其为非法的
    size_t dot_dot_res = path.find("..");
    if (dot_dot_res == string::npos) // 没有找到
    {
        // 拼接路径
        const string file_path = string("./resources/") + path;
        // 检查是否有指定的文件
        int fp = open(file_path.c_str(), ios::in | ios::binary);
        if (fp > 0) // 找到了文件
        {
            //cout << "founded " << file_path << endl;
            return file_sender(session, fp);
        }
        else
        {
            //cout << "not founded " << file_path << endl;
        }
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
