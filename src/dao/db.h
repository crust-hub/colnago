#pragma once

#include <memory>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>

namespace colnago
{
    namespace db
    {
        extern std::auto_ptr<odb::core::database> db;
    }
}
