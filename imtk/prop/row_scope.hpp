#pragma once

#include "imtk/edit_session.hpp"
#include "imtk/list_model.hpp"

#include "imtk/prop/property_grid.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace imtk::prop
{
    struct iresettable
    {
        virtual ~iresettable() = default;

        virtual bool out_of_sync() const = 0;
        virtual void reset() = 0;
    };

    class row_scope
    {
        bool valid_;
        std::unique_ptr<iresettable> resetter_;

    public:
        row_scope(std::string_view label, std::unique_ptr<iresettable> resetter);
        row_scope(row_scope&& o) noexcept;
        ~row_scope();

        row_scope(const row_scope&) = delete;
        row_scope& operator=(row_scope&&) = delete;

        operator bool() const;
    };

    class multi_row_scope
    {
        bool valid_;
        std::vector<std::unique_ptr<iresettable>> resetters_;

    public:
        multi_row_scope(std::string_view label, std::vector<std::unique_ptr<iresettable>> resetters);
        multi_row_scope(multi_row_scope&& o) noexcept;
        ~multi_row_scope();

        multi_row_scope(const multi_row_scope&) = delete;
        multi_row_scope& operator=(multi_row_scope&&) = delete;

        operator bool() const;
    };

    template<typename ty>
    class vector_row_scope
    {
        edit_session<std::vector<ty>>& data_;
        const std::vector<ty>& def_;
        list_model& model_;

    public:
        vector_row_scope(
            std::string_view label,
            edit_session<std::vector<ty>>& data,
            const std::vector<ty>& def,
            list_model& model)
            : data_(data)
            , def_(def)
            , model_(model)
        {
            key::set_label(label);
            data_.pre_edit();
            if (data_.buffer().size() != def_.size())
                reset::button(0);

            for (size_t i = 0; i < data_.buffer().size(); ++i)
            {
                if (i < def_.size())
                {
                    if (data_.buffer()[i] != def_[i])
                        reset::button(1 + i);
                }
                else
                {
                    if (data_.buffer()[i] != ty{})
                        reset::button(1 + i);
                }
            }
        }

        ~vector_row_scope()
        {
            row::submit();

            data_.post_edit(row::get_draw_result().state);
            if (reset::activated(0))
                model_.defer_resize(def_.size());

            std::vector<std::string> reset = data_.buffer();
            bool publish = false;

            for (size_t i = 0; i < data_.buffer().size(); ++i)
            {
                if (reset::activated(1 + i))
                {
                    reset[i] = i < def_.size() ? def_[i] : ty{};
                    publish = true;
                }
            }

            if (publish)
                data_.publish_reset(std::move(reset));
        }

        vector_row_scope(const vector_row_scope&) = delete;
        vector_row_scope(vector_row_scope&&) = delete;

        operator bool() const
        {
            return true;
        }
    };
}
