#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <regex>
#include <sqlite3.h>
#include <chrono>
#include "router/dynamic/image.h"
#include "entity/response.h"
#include "utils/multipart_form_data_parser.h"
#include "server/m_server.h"

using namespace colnago::router;
using namespace std;
using namespace colnago::entity;
using namespace colnago::utils;

static const regex image_regex(string("image/.*"));

/**
 * @brief POST /image
 *
 * @param session
 */
void image::POST(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    long long int content_length = stoll(request->get_header("Content-Length", "0"));
    string content_type = request->get_header("Content-Type");
    auto lines = multipart_form_data_parser::boundary(content_type);
    const string slice_line = lines.first;
    const string end_line = lines.second;
    auto handler = [slice_line, end_line, content_type](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
    {
        if (body.size() > 118207938)
        {
            session->close(restbed::NOT_FOUND, "oversize", {{"Content-Type", "text/text"}});
            return;
        }

        istringstream body_stream(string(body.begin(), body.end()));
        auto form_data = std::move(multipart_form_data_parser::parser(body_stream, slice_line, end_line));

        for (size_t i = 0; i < form_data.first.size(); i++)
        {
            auto &header = form_data.first[i];
            // for (auto &kv : header)
            // {
            //     cout << kv.first << "," << kv.second << endl;
            // }
            // cout << header.at("Content-Disposition") << endl;
            if (header.find("Content-Type") != header.end()) //有Content-Type
            {
                auto const &content_type = header.at("Content-Type");
                if (regex_match(content_type, image_regex)) //是图片
                {
                    sqlite3_stmt *stmt; //声明
                    chrono::steady_clock::time_point time1 = chrono::steady_clock::now();
                    const string id = to_string(time1.time_since_epoch().count());
                    const string sql = "insert into image(ID,DATA,TYPE) values(?,?,?)";
                    sqlite3_prepare(colnago::server::server.db->db, sql.c_str(), sql.length(), &stmt, 0);
                    {
                        sqlite3_bind_text(stmt, 1, id.c_str(), id.length(), SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt, 3, content_type.c_str(), content_type.length(), SQLITE_TRANSIENT);
                        sqlite3_bind_blob(stmt, 2, form_data.second[i].c_str(), form_data.second[i].length(), SQLITE_TRANSIENT);
                        sqlite3_step(stmt);
                        sqlite3_finalize(stmt);
                    }
                    BaseResponse<string> base_response;
                    base_response.message = "插入成功";
                    base_response.result = true;
                    base_response.m_list.push_back(id);
                    string json_res = base_response.stringify([](string &item) -> string
                                                              { return item; });
                    session->close(restbed::OK, json_res, ResponseHeader::Base(ResponseHeader::JSON));
                    return;
                }
            }
        }
        session->close(restbed::NOT_MODIFIED);
    };
    session->fetch(content_length, handler);
}

/**
 * @brief 获取图片
 *
 * @param session
 */
void image::GET(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    const string id = request->get_query_parameter("id");
    sqlite3_stmt *stmt; //声明
    const string sql = "select * from image where ID =?;";
    sqlite3_prepare(colnago::server::server.db->db, sql.c_str(), sql.length(), &stmt, 0);
    {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    }
    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        const char *type = (char *)sqlite3_column_blob(stmt, 2);
        const char *data = (char *)sqlite3_column_blob(stmt, 1);
        restbed::Bytes bytes;
        size_t len = sqlite3_column_bytes(stmt, 1); //返回数据大小
        for (size_t i = 0; i < len; i++)
        {
            bytes.push_back(data[i]);
        }
        const string str_type(type);
        session->close(restbed::OK, bytes, {{"Content-Type", str_type}, {"Content-Length", to_string(len)}});
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt); //把刚才分配的内容析构掉
    session->close(restbed::NOT_FOUND);
}

std::shared_ptr<restbed::Resource> image::resource()
{
    const char *postRestFul = "/image";
    auto resource = make_shared<restbed::Resource>();
    resource->set_path(postRestFul);
    resource->set_method_handler("POST", POST);
    resource->set_method_handler("GET", GET);
    return resource;
}
