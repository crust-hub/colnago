#include"view/asset.h"
using namespace colnago::view;

std::string Asset::source(const std::string& path){
    return std::string("../src/view/asset/") + path;
}