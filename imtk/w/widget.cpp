#include "widget.hpp"

#include <imgui.h>

namespace imtk::w
{
    item_result widget::result() const
    {
        return result_;
    }

    item_result widget::draw()
    {
        result_ = draw_impl();
        return result_;
    }

    item_result widget::prefix_label(std::string_view label)
    {
        item_result result;

        if (!label.empty())
        {
            ImGui::TextUnformatted(label.data(), label.data() + label.size());
            result |= item_result::query(false);
            ImGui::SameLine();
        }

        return result;
    }
}
