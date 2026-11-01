#pragma once

#include "imtk/datapath.hpp"
#include "imtk/pkey.hpp"

#include <unordered_map>

namespace imtk::desc
{
    template<typename key_ty, typename d>
    class map
    {
        std::unordered_map<key_ty, d> map_;

    public:
        datapath_link link;
        pkey key;

    public:
        map(pkey key, datapath_link link)
            : link(std::move(link))
            , key(key)
        {
        }

        void clear()
        {
            map_.clear();
        }

        d& operator[](key_ty key)
        {
            auto it = map_.find(key);
            if (it != map_.end())
                return it->second;
            else
                return map_.emplace(key, datapath_link(link, datapath::step(key))).first->second;
        }

        auto begin()
        {
            return map_.begin();
        }

        auto end()
        {
            return map_.end();
        }

        void* resolve(datapath_view path, imp::type_erasure type)
        {
            if (path.empty())
                return imp::resolve_type(type, this);

            auto it = map_.find(static_cast<key_ty>((int)path.step()));
            if (it != map_.end())
                return it->second.resolve(path.next(), type);
            else
                return nullptr;
        }

        void describe(std::ostream& os, datapath_view path) const
        {
            if (path.empty())
                os << "<error>";
            else
            {
                auto k = static_cast<key_ty>((int)path.step());
                auto it = map_.find(k);
                if (it != map_.end())
                {
                    path = path.next();
                    if (path.empty())
                        os << k;
                    else
                    {
                        os << k << ".";
                        it->second.describe(os, path);
                    }
                }
                else
                    os << "<error>";
            }
        }

        bool query_dirty(const map<key_ty, d>& disk) const
        {
            if (map_.size() != disk.map_.size())
                return true;

            for (const auto& [key, desc] : map_)
            {
                auto it = disk.map_.find(key);
                if (it == disk.map_.end())
                    return true;

                if (desc.query_dirty(it->second))
                    return true;
            }

            return false;
        }

        void copy_data(const map<key_ty, d>& o)
        {
            for (auto it = map_.begin(); it != map_.end(); ++it)
            {
                auto o_it = o.map_.find(it->first);
                if (o_it != o.map_.end())
                    it->second.copy_data(o_it->second);
                else
                    it = map_.erase(it);
            }

            for (const auto& [o_key, o_desc] : o.map_)
            {
                if (!map_.contains(o_key))
                    (*this)[o_key].copy_data(o_desc);
            }
        }

        toml_node subnode(toml_node node) const
        {
            return node[encode_pkey(key)];
        }

        template<typename value_ty>
        void dump_into(toml::table& table, value_ty&& value) const
        {
            table.insert_or_assign(encode_pkey(key), std::forward<value_ty>(value));
        }
    };
}
