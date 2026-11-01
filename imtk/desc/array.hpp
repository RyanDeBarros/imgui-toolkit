#pragma once

#include "imtk/datapath.hpp"
#include "imtk/pkey.hpp"

namespace imtk::desc
{
    template<typename d, size_t n>
    class array
    {
    public:
        datapath_link link;
        pkey key;

    private:
        std::array<d, n> array_;

    public:
        array(pkey key, datapath_link link)
            : link(std::move(link)), key(key)
            , array_(init_array(this->link, std::make_index_sequence<n>{}))
        {
        }

    public:
        size_t size() const
        {
            return n;
        }

        d& operator[](size_t i)
        {
            return array_[i];
        }

        const d& operator[](size_t i) const
        {
            return array_[i];
        }

        auto begin() const
        {
            return array_.begin();
        }

        auto begin()
        {
            return array_.begin();
        }

        auto end() const
        {
            return array_.end();
        }

        auto end()
        {
            return array_.end();
        }

        void* resolve(datapath_view path, imp::type_erasure type)
        {
            if (path.empty())
                return imp::resolve_type(type, this);

            int index = path.step();
            if (index >= 0 && index < n)
                return array_[index].resolve(path.next(), type);
            else
                return nullptr;
        }

        void describe(std::ostream& os, datapath_view path) const
        {
            if (path.empty())
                os << "<error>";
            else
            {
                int index = path.step();
                if (index >= 0 && index < n)
                {
                    path = path.next();
                    if (path.empty())
                        os << index;
                    else
                    {
                        os << index << ".";
                        array_[index].describe(os, path);
                    }
                }
                else
                    os << "<error>";
            }
        }

        bool query_dirty(const array<d, n>& disk) const
        {
            for (size_t i = 0; i < n; ++i)
            {
                if (array_[i].query_dirty(disk.array_[i]))
                    return true;
            }

            return false;
        }

        void copy_data(const array<d, n>& o)
        {
            for (size_t i = 0; i < n; ++i)
                array_[i].copy_data(o.array_[i]);
        }

        // TODO macros to generate subnode/dump_into ?

        toml_node subnode(toml_node node) const
        {
            return node[encode_pkey(key)];
        }

        template<typename value_ty>
        void dump_into(toml::table& table, value_ty&& value) const
        {
            table.insert_or_assign(encode_pkey(key), std::forward<value_ty>(value));
        }

    private:
        template<size_t... i>
        static auto init_array(datapath_link& link, std::index_sequence<i...>)
        {
            return std::array<d, n>{ d(datapath_link(link, datapath::step(i)))... };
        }
    };
}
