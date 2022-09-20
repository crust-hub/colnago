#include "router/other/static_router.h"

#include <memory>
#include "view/asset.h"
#include "view/view.h"
#include "nlohmann/json.hpp"
#include "entity/response.h"

using namespace colnago::router::other;
using namespace colnago::entity;
using namespace colnago::view;

void static_router::default_event(const shared_ptr<Session> session)
{
    if (session->is_open() == false)
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