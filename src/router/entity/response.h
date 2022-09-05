#pragma once

#include <string>
#include <list>
#include <nlohmann/json.hpp>

namespace colnago
{
    namespace entity
    {
        template <typename T = char>
        class BaseResponse
        {
        public:
            bool result;
            std::string message;
            std::list<T> m_list;
            BaseResponse(bool result = true, std::string message = "");
            BaseResponse(bool result, std::string message, const std::list<T> &t);
            std::string stringify(std::string (*callback)(T &t));
            std::string stringify();
        };
    }
}

namespace colnago
{
    namespace entity
    {
        using json = nlohmann::json;
        template <typename T>
        BaseResponse<T>::BaseResponse(bool result, std::string message) : result(result), message(message)
        {
        }

        template <typename T>
        BaseResponse<T>::BaseResponse(bool result, std::string message, const std::list<T> &t) : result(result), message(message), m_list(t)
        {
        }

        template <typename T>
        std::string BaseResponse<T>::stringify(std::string (*callback)(T &t))
        {
            auto res = R"({
                    "result":true,
                    "message":"",
                    "list":[]
                })"_json;
            res["result"] = result;
            res["message"] = message;
            json array = json::array();
            for (auto item : m_list)
            {
                array.push_back(json::parse(callback(item)));
            }
            res["list"] = array;
            return res.dump();
        }

        template <typename T>
        std::string BaseResponse<T>::stringify()
        {
            auto res = R"({
                    "result":true,
                    "message":""
                })"_json;
            res["result"] = result;
            res["message"] = message;
            return res.dump();
        }
    }
}
