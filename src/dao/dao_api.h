#pragma once
#include <sqlite3.h>
namespace colnago
{
    namespace dao
    {
        class DaoAPI
        {
        public:
            virtual void init() = 0;
            sqlite3 *db;
        };
    }
}