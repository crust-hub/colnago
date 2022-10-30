#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
namespace colnago
{
    namespace view
    {
        extern inja::Environment env;
        std::string render(const char *path, nlohmann::json data);
    }
}