#include <memory>
#include <iostream>
#include "entity/response.h"

using namespace colnago::entity;
using namespace std;

const multimap<string, string> ResponseHeader::Base(ResponseHeader::type t)
{
    //static shared_ptr<multimap<string, string>> res;
    string type;
    switch (t)
    {
    case ResponseHeader::JSON:
        type = "application/json";
        break;
    case ResponseHeader::HTML:
        type = "text/html";
        break;
    default:
        type = "text/text";
        break;
    }
    /*
    if (res == nullptr)
    {
        const multimap<string, string> m_map = {{"Content-Type", type}};
        res = make_shared<multimap<string, string>>(m_map);
    }
    return *res.get();
    */
    const multimap<string,string> m_map={{"Content-Type",type}};
    return m_map;
}
