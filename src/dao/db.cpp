#include "dao/db.h"

std::auto_ptr<odb::core::database> colnago::db::db = std::auto_ptr<odb::core::database>(new odb::mysql::database("root", "", "colnago", "localhost", 3306));
