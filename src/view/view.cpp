#include "view.h"
#include <inja/inja.hpp>
namespace colnago
{
    namespace view
    {
        inja::Environment env;
        std::string render(const char *path, nlohmann::json data)
        {

            std::string result = env.render_file(path, data);
            return result;
        }
    }
}