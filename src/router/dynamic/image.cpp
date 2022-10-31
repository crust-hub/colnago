#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
#include <nlohmann/json.hpp>
#include <odb/mysql/exceptions.hxx>
#include "router/dynamic/image.h"
#include "entity/response.h"
#include "utils/multipart_form_data_parser.h"
#include "server/server.h"
#include "dao/image/image-odb.hxx"
#include "dao/image/image-odb.ixx"
#include "dao/image/image.h"
#include "dao/db.h"
#include "service/service.h"

using namespace std;
using namespace colnago::entity;
using namespace colnago::utils;
using namespace colnago::service;
using namespace colnago::db;

static const regex image_regex(string("image/.*"));

/**
 * @brief POST /image
 *
 * @param session
 */
void colnago::router::ImageController::POST(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    long long int content_length = stoll(request->get_header("Content-Length", "0"));
    string content_type = request->get_header("Content-Type");
    auto lines = multipart_form_data_parser::boundary(content_type);
    const string slice_line = lines.first;
    const string end_line = lines.second;
    static constexpr size_t max_size = 1024 * 1024 * 3;
    auto handler = [slice_line, end_line, content_type](const std::shared_ptr<restbed::Session> session, const restbed::Bytes &body) -> void
    {
        if (body.size() > max_size) // 3MB
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
                    vector<char> buffer(form_data.second[i].begin(), form_data.second[i].end());
                    chrono::steady_clock::time_point time1 = chrono::steady_clock::now();
                    Image image(time1.time_since_epoch().count(), buffer, content_type);
                    long long id = -1;
                    string message = "";
                    odb::transaction t(db::db->begin());
                    try
                    {
                        id = Service<Image>::add(image, image.id()); //存储图片数据
                    }
                    catch (odb::mysql::database_exception &e)
                    {
                        message = e.what();
                    }
                    t.commit();
                    BaseResponse<string> base_response;
                    base_response.message = message;
                    base_response.result = (id > 0);
                    base_response.m_list.push_back(to_string(id));
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
void colnago::router::ImageController::GET(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    const string id = request->get_query_parameter("id");
    odb::transaction t(db::db->begin());
    auto res = Service<Image>::getById(stoll(id));
    if (res->empty())
    {
        session->close(restbed::NOT_FOUND);
        t.commit();
        return;
    }
    restbed::Bytes back;
    const vector<char> &data = res->begin()->data();
    for (auto &item : data)
    {
        back.push_back(item);
    }
    session->close(restbed::OK,
                   back, {{"Content-Type", res->begin()->type()}, {"Content-Length", to_string(res->begin()->data().size())}});
    t.commit();
    return;
}

/**
 * @brief Get the list object
 *
 * @param session
 */
void colnago::router::ImageController::GET_LIST(const std::shared_ptr<restbed::Session> session)
{
    const auto request = session->get_request();
    auto page_str = request->get_query_parameter("page");
    long long int page = 1;
    if (page_str != "")
    {
        page = stoll(page_str);
    }
    odb::transaction t(db::db->begin());
    auto res_list = Service<Image>::page(odb::query<Image>::id.is_not_null(), 10, page);
    t.commit();
    BaseResponse<Image> base_response(true, "检索成功", *res_list);
    auto res = base_response.stringify([](Image &image) -> std::string
                                       { 
                                        json j = json::parse(R"({"url":""})");
                                        j["url"] = string("/image?id=") + to_string(image.id());
                                        return j.dump(); });
    session->close(restbed::OK, res, ResponseHeader::Base(ResponseHeader::JSON));
    return;
}

std::shared_ptr<restbed::Resource> colnago::router::ImageController::resource(std::shared_ptr<restbed::Service> service)
{
    const char *postRestFul = "/image";
    auto resource = make_shared<restbed::Resource>();
    resource->set_path(postRestFul);
    resource->set_method_handler("POST", ImageController::POST);
    resource->set_method_handler("GET", ImageController::GET);
    auto resource_list = make_shared<restbed::Resource>();
    resource_list->set_path("/image/list");
    resource_list->set_method_handler("GET", ImageController::GET_LIST);
    service->publish(resource);
    service->publish(resource_list);
    return resource;
}
