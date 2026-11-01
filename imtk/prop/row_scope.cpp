#include "row_scope.hpp"

namespace imtk::prop
{
    row_scope::row_scope(std::string_view label, std::unique_ptr<iresettable> resetter)
        : valid_(true)
        , resetter_(std::move(resetter))
    {
        key::set_label(label);
        if (resetter_->out_of_sync())
            reset::button();
    }

    row_scope::row_scope(row_scope&& o) noexcept
        : valid_(o.valid_)
        , resetter_(std::move(o.resetter_))
    {
        o.valid_ = false;
    }

    row_scope::~row_scope()
    {
        if (valid_)
        {
            row::submit();
            if (reset::any_activated())
                resetter_->reset();
        }
    }

    row_scope::operator bool() const
    {
        return valid_;
    }

    multi_row_scope::multi_row_scope(std::string_view label, std::vector<std::unique_ptr<iresettable>> resetters)
        : valid_(true)
        , resetters_(std::move(resetters))
    {
        key::set_label(label);
        for (size_t i = 0; i < resetters_.size(); ++i)
        {
            if (resetters_[i]->out_of_sync())
                reset::button(i);
        }
    }

    multi_row_scope::multi_row_scope(multi_row_scope&& o) noexcept
        : valid_(o.valid_), resetters_(std::move(o.resetters_))
    {
        o.valid_ = false;
    }

    multi_row_scope::~multi_row_scope()
    {
        if (valid_)
        {
            row::submit();
            for (size_t i = 0; i < resetters_.size(); ++i)
            {
                if (reset::activated(i))
                    resetters_[i]->reset();
            }
        }
    }

    multi_row_scope::operator bool() const
    {
        return valid_;
    }
}
