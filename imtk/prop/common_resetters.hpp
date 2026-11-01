#pragma once

#include "imtk/desc/vector.hpp"

#include "imtk/prop/row_scope.hpp"

namespace imtk::prop
{
    template<typename ty, typename def_ty>
    class resettable_value
        : public iresettable
    {
        ty& data_;
        const def_ty& _def;

    public:
        resettable_value(ty& data, const def_ty& def)
            : data_(data)
            , _def(def)
        {
        }

        bool out_of_sync() const override
        {
            return data_ != _def;
        }

        void reset() override
        {
            data_ = _def;
        }
    };

    template<typename ty, typename def_ty>
    class resettable_value<edit_session<ty>, def_ty>
        : public iresettable
    {
        edit_session<ty>& data_;
        const def_ty& _def;

    public:
        resettable_value(edit_session<ty>& data, const def_ty& def)
            : data_(data)
            , _def(def)
        {
        }

        bool out_of_sync() const override
        {
            data_.pre_edit();
            return data_.buffer() != _def;
        }

        void reset() override
        {
            data_.publish_reset(_def);
        }
    };

    template<typename ty, typename def_ty>
    auto make_resettable_value(ty& data, const def_ty& def)
    {
        return std::make_unique<resettable_value<ty, def_ty>>(data, def);
    }

    template<typename ty, typename def_ty>
    auto make_resettable_value(edit_session<ty>& data, const def_ty& def)
    {
        return std::make_unique<resettable_value<edit_session<ty>, def_ty>>(data, def);
    }

    class resettable_row
        : public iresettable
    {
        std::vector<std::unique_ptr<iresettable>> resetters_;

    public:
        resettable_row(std::vector<std::unique_ptr<iresettable>> resetters)
            : resetters_(std::move(resetters))
        {
        }

        bool out_of_sync() const override
        {
            bool any = false;
            for (auto& r : resetters_)
                any |= r->out_of_sync();
            return any;
        }

        void reset() override
        {
            for (auto& r : resetters_)
                r->reset();
        }
    };

    template<typename... ty>
    std::unique_ptr<resettable_row> make_resettable_value_row(ty&&... resetters)
    {
        std::vector<std::unique_ptr<iresettable>> values;
        values.reserve(sizeof...(resetters));
        (values.push_back(std::forward<ty>(resetters)), ...);
        return std::make_unique<resettable_row>(std::move(values));
    }

    template<typename ty>
    class resettable_vector_size
        : public iresettable
    {
        list_model& model_;
        desc::vector<ty>& data_;
        size_t def_size_;

    public:
        resettable_vector_size(list_model& model, desc::vector<ty>& data, size_t def_size)
            : model_(model)
            , data_(data)
            , def_size_(def_size)
        {
        }

        bool out_of_sync() const override
        {
            return data_.size() != def_size_;
        }

        void reset() override
        {
            model_.defer_resize(def_size_);
        }
    };

    template<typename ty, typename def_ty>
    row_scope make_row_scope(std::string_view label, ty& data, const def_ty& def)
    {
        return row_scope(label, make_resettable_value(data, def));
    }

    template<typename ty, typename def_ty>
    row_scope make_row_scope(std::string_view label, edit_session<ty>& data, const def_ty& def)
    {
        return row_scope(label, make_resettable_value(data, def));
    }
}
