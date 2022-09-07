#include <memory>
#include "router/entity/response.h"

using namespace colnago::entity;
using namespace std;

const multimap<string, string> &ResponseHeader::JSON()
{
    static shared_ptr<multimap<string, string>> res;
    if (res == nullptr)
    {
        const multimap<string, string> m_map = {{"Content-Type", "application/json"}};
        res = make_shared<multimap<string, string>>(m_map);
    }
    return *res.get();
}
