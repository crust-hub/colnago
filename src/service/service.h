#include <list>
#include <memory.h>

#include "dao/db.h"

namespace colnago
{
    namespace service
    {
        using namespace std;
        template <typename T>
        class Service
        {
        private:
            Service() = default;

        public:
            static shared_ptr<list<T>> getAll();

            static void update(T &t);

            static shared_ptr<list<T>> query(odb::query<T> q);

            template <typename ID>
            static void deleteById(ID id)
            {
                db::db->erase<T>(id);
            }

            template <typename ID>
            static shared_ptr<list<T>> getById(ID id)
            {
                odb::query<T> query(odb::query<T>::id == id);
                shared_ptr<list<T>> resList = make_shared<list<T>>();
                odb::result<T> res(db::db->query(query));
                for (auto &item : res)
                {
                    resList->push_back(item);
                }
                return resList;
            }

            template <typename ID>
            static ID add(T &t, ID id)
            {
                ID auto_id = db::db->persist(t);
                return auto_id;
            };

            static shared_ptr<list<T>> page(odb::query<T> q_base, int page_size, unsigned page)
            {
                page = page < 1 ? 1 : page;
                long long start = (page - 1) * page_size;
                odb::query<T> q(
                    q_base +
                    "LIMIT" +
                    odb::query<T>::_ref(page_size) +
                    +"OFFSET" +
                    odb::query<T>::_ref(start));
                return query(q);
            }
        };

        template <typename T>
        shared_ptr<list<T>> Service<T>::getAll()
        {
            shared_ptr<list<T>> res_list = make_shared<list<T>>();
            odb::result<T> all(db::db->query<T>());
            for (auto &item : all)
            {
                res_list->push_back(item);
            }
            return res_list;
        }

        template <typename T>
        void Service<T>::update(T &t)
        {
            db::db->update(t);
        }

        template <typename T>
        shared_ptr<list<T>> Service<T>::query(odb::query<T> q)
        {
            odb::result<T> res(db::db->query(q));
            shared_ptr<list<T>> res_list = make_shared<list<T>>();
            for (auto &item : res)
            {
                res_list->push_back(item);
            }
            return res_list;
        }

    }
}