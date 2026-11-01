#pragma once

#include "imtk/datapath.hpp"
#include "imtk/list_model.hpp"
#include "imtk/pkey.hpp"

#include "imtk/prop/property_grid.hpp"

namespace imtk::desc
{
    template<typename d>
    class vector
    {
        std::vector<d> vector_;

    public:
        datapath_link link;
        pkey key;

    public:
        vector(pkey key, datapath_link link)
            : link(std::move(link))
            , key(key)
        {
        }

        d& push_back()
        {
            vector_.push_back(d(datapath_link(link, datapath::step(vector_.size()))));
            return vector_.back();
        }

        void insert(size_t i, d element)
        {
            for (auto it = vector_.begin() + i; it != vector_.end(); ++it)
                it->link.set_step(*it->link.step() + 1);

            element.link.parent_to(link, datapath::step(i));
            vector_.insert(vector_.begin() + i, std::move(element));
        }

        void remove(size_t i)
        {
            vector_.erase(vector_.begin() + i);
            for (auto it = vector_.begin() + i; it != vector_.end(); ++it)
                it->link.set_step(*it->link.step() - 1);
        }

        void clear()
        {
            vector_.clear();
        }

        size_t size() const
        {
            return vector_.size();
        }

        void resize(size_t new_size)
        {
            if (new_size < vector_.size())
                vector_.erase(vector_.begin() + new_size, vector_.end());
            else if (new_size > vector_.size())
            {
                for (size_t i = vector_.size(); i < new_size; ++i)
                    vector_.push_back(d(datapath_link(link, datapath::step(i))));
            }
        }

        bool empty() const
        {
            return vector_.empty();
        }

        d& back()
        {
            return vector_.back();
        }

        const d& back() const
        {
            return vector_.back();
        }

        d& operator[](size_t i)
        {
            return vector_[i];
        }

        const d& operator[](size_t i) const
        {
            return vector_[i];
        }

        auto begin() const
        {
            return vector_.begin();
        }

        auto begin()
        {
            return vector_.begin();
        }

        auto end() const
        {
            return vector_.end();
        }

        auto end()
        {
            return vector_.end();
        }

        void* resolve(datapath_view path, imp::type_erasure type)
        {
            if (path.empty())
                return imp::resolve_type(type, this);

            int index = path.step();
            if (index >= 0 && index < vector_.size())
                return vector_[index].resolve(path.next(), type);
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
                if (index >= 0 && index < vector_.size())
                {
                    path = path.next();
                    if (path.empty())
                        os << index;
                    else
                    {
                        os << index << ".";
                        vector_[index].describe(os, path);
                    }
                }
                else
                    os << "<error>";
            }
        }

        bool query_dirty(const vector<d>& disk) const
        {
            if (vector_.size() != disk.vector_.size())
                return true;

            for (size_t i = 0; i < vector_.size(); ++i)
            {
                if (vector_[i].query_dirty(disk.vector_[i]))
                    return true;
            }

            return false;
        }

        void copy_data(const vector<d>& o)
        {
            resize(o.size());
            for (size_t i = 0; i < vector_.size(); ++i)
                vector_[i].copy_data(o.vector_[i]);
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

        struct list_op_adapter
            : public ilist_op_adapter
        {
            vector<d>& desc;

            list_op_adapter(vector<d>& desc)
                : desc(desc)
            {
            }

            void apply(const list_op& op) const override
            {
                op.execute_desc_action<d>(desc.link.compute_path());
            }
        };

        void consume_ops(list_model& model)
        {
            if (model.consume_ops(list_op_adapter(*this)))
                prop::grid::mark_dirty();
        }
    };
}
