#include "dao/db.h"

std::auto_ptr<odb::core::database> colnago::db::db = std::auto_ptr<odb::core::database>(new odb::mysql::database("root", "Gao13346637702.", "colnago", "61.171.51.135", 3306));
