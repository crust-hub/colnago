#include "utils/util.h"
using namespace colnago::utils;
using namespace std;

bool util::string_start_with(const std::string &f, const std::string &sub)
{
    if (sub.empty())
        return true;
    if (f.size() < sub.size())
        return false;
    for (size_t i = 0; i < sub.size(); i++)
    {
        if (sub[i] != f[i])
        {
            return false;
        }
    }
    return true;
}
