#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "router/dynamic/image.h"
#include "entity/response.h"
#include "utils/multipart_form_data_parser.h"

using namespace colnago::router;
using namespace std;
using namespace colnago::entity;
using namespace colnago::utils;

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
        auto form_data = multipart_form_data_parser::parser(body_stream, slice_line, end_line);

        for (size_t i = 0; i < form_data.first.size(); i++)
        {
            auto &header = form_data.first[i];
            if (header.find("Content-Type") != header.end())
            {
                cout << header.at("Content-Type") << endl;
                cout << "size: " << form_data.second[i].size() << endl;
                if (header.at("Content-Type") == "image/jpeg\r" || header.at("Content-Type") == "image/png\r")
                {
                    session->close(restbed::OK, form_data.second[i], {{"Content-Type", header.at("Content-Type")}});
                    return;
                }
            }
        }

        session->close(restbed::OK, "post success", {{"Content-Type", "text/text"}});
    };
    session->fetch(content_length, handler);
}

std::shared_ptr<restbed::Resource> image::resource()
{
    const char *postRestFul = "/image";
    auto resource = make_shared<restbed::Resource>();
    resource->set_path(postRestFul);
    resource->set_method_handler("POST", POST);
    return resource;
}