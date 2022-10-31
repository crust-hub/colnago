#include "dao/db.h"

std::auto_ptr<odb::core::database> colnago::db::db = std::auto_ptr<odb::core::database>(new odb::mysql::database("root", "", "colnago", "", 3306));

// std::auto_ptr<odb::core::database> colnago::db::db = std::auto_ptr<odb::core::database>(new odb::mysql::database("root", "gaowanlu", "colnago", "127.0.0.1", 3306));
