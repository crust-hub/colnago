#pragma once
#include <sqlite3.h>
#include <cstdio>
#include <iostream>
#include "dao/dao_api.h"
#include <memory>
namespace colnago
{
    namespace dao
    {
        class ColnagoDB : public DaoAPI
        {
        public:
            static std::shared_ptr<colnago::dao::DaoAPI> create(const char *path)
            {
                std::shared_ptr<colnago::dao::DaoAPI> ptr = std::make_shared<ColnagoDB>(path);
                ptr->init();
                return ptr;
            }
            ColnagoDB(const char *path);
            ~ColnagoDB();
            void init() override;
        };
    }
}