#include "value_outline.hpp"

#include "imtk/prop/property_grid.hpp"

namespace imtk::prop
{
    value_outline::value_outline()
    {
        begin_listener_ = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
        end_listener_ = value::on_value_end().subscribe([this]() { on_end_outline(); });
    }

    value_outline::value_outline(const value_outline& o)
        : begin_(o.begin_), end_(o.end_)
    {
        begin_listener_ = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
        end_listener_ = value::on_value_end().subscribe([this]() { on_end_outline(); });
    }

    value_outline::value_outline(value_outline&& o) noexcept
        : begin_(o.begin_), end_(o.end_)
    {
        begin_listener_ = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
        end_listener_ = value::on_value_end().subscribe([this]() { on_end_outline(); });
    }

    value_outline& value_outline::operator=(const value_outline& o)
    {
        if (this != &o)
        {
            begin_ = o.begin_;
            end_ = o.end_;
        }

        return *this;
    }

    value_outline& value_outline::operator=(value_outline&& o) noexcept
    {
        if (this != &o)
        {
            begin_ = o.begin_;
            end_ = o.end_;
        }

        return *this;
    }

    value_outline::operator bool() const
    {
        return true;
    }

    void value_outline::draw(ImU32 color, outline::config cfg) const
    {
        ImGui::GetWindowDrawList()->AddRect(begin_, end_, color, cfg.rounding, cfg.flags, cfg.border);
    }

    void value_outline::kill()
    {
        begin_listener_.detach();
        end_listener_.detach();
    }

    void value_outline::on_begin_outline()
    {
        begin_ = ImGui::GetCursorScreenPos();
    }

    void value_outline::on_end_outline()
    {
        end_ = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMin().y + ImGui::GetFrameHeight() };
    }
}
