#pragma once
#include <string>
#include <list>
#include <utility>
#include <tuple>
#include <map>
#include <nlohmann/json.hpp>

namespace colnago
{
    namespace entity
    {
        template <typename T = std::string>
        class BaseResponse
        {
        public:
            bool result;
            std::string message;
            std::list<T> m_list;
            BaseResponse(bool result = true, std::string message = "");
            BaseResponse(bool result, std::string message, const std::list<T> &t);
            BaseResponse(std::pair<bool, std::string> m_pair);
            BaseResponse(std::tuple<bool, std::string, std::list<T>> m_tuple);
            std::string stringify(std::string (*callback)(T &t));
            std::string stringify();
        };

        class ResponseHeader
        {
        public:
            enum type
            {
                JSON = 0,
                HTML
            };
            static const std::multimap<std::string, std::string> Base(type t);
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
        BaseResponse<T>::BaseResponse(std::pair<bool, std::string> m_pair)
        {
            result = m_pair.first;
            message = m_pair.second;
        }

        template <typename T>
        BaseResponse<T>::BaseResponse(std::tuple<bool, std::string, std::list<T>> m_tuple)
        {
            *this = BaseResponse<T>(std::get<0>(m_tuple), std::get<1>(m_tuple), std::get<2>(m_tuple));
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
            json array = json::array();
            for (auto &item : m_list)
            {
                array.push_back(item);
            }
            res["list"] = array;
            return res.dump();
        }
    }
}