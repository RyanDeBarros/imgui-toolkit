#include "common_widgets.hpp"

#include "imtk/controls.hpp"
#include "imtk/id_scope.hpp"
#include "imtk/simple_scopes.hpp"

#include "imtk/prop/common_views.hpp"
#include "imtk/prop/property_grid.hpp"

namespace imtk::w
{
    generic_widget::generic_widget(std::function<item_result()> draw_fn)
        : draw_fn(std::move(draw_fn))
    {
    }

    item_result generic_widget::draw_impl()
    {
        return draw_fn();
    }

    bound_optional::bound_optional(
        std::unique_ptr<widget>&& item,
        bool& enable,
        bound_widget<bool>::config_impl config)
        : enable(enable, std::move(config))
        , item(std::move(item))
    {
    }

    item_result bound_optional::draw_impl()
    {
        auto result = enable.draw();
        if (auto d = disabled(!enable.data))
        {
            ImGui::SameLine();
            result |= item->draw();
        }
        return result;
    }

    simple_optional::simple_optional(
        std::unique_ptr<widget>&& item,
        bool& enable,
        simple_widget<bool>::config_impl config)
        : enable(enable, std::move(config))
        , item(std::move(item))
    {
    }

    item_result simple_optional::draw_impl()
    {
        auto result = enable.draw();
        if (auto d = disabled(!enable.value))
        {
            ImGui::SameLine();
            result |= item->draw();
        }
        return result;
    }

    combo_widget::combo_widget(int& index, label_span_handle names, config_impl config)
        : index(index)
        , names(names)
        , config(std::move(config))
    {
    }

    item_result combo_widget::draw_impl()
    {
        id_scope scope(&index);
        auto result = prefix_label(config.label.c_str());

        result |= item_result::query(ImGui::Combo("", &index, &label_span_registry::combo_getter, &names, names.count()));

        result.modified |= prop::grid::check_property(std::make_unique<prop::combo_view>(index, names));
        return result;
    }

    readonly_text::readonly_text(std::string& text, config_impl config)
        : text(text)
        , config(std::move(config))
    {
    }

    item_result readonly_text::draw_impl()
    {
        id_scope scope(text.data());
        return prefix_label(config.label.c_str()) | controls::readonly_text("", text, config.flags);
    }

    readonly_text_owned::readonly_text_owned(std::string text, readonly_text::config_impl config)
        : text(std::move(text))
        , view(this->text, std::move(config))
    {
    }

    item_result readonly_text_owned::draw_impl()
    {
        return view.draw();
    }

    disabler::disabler(std::unique_ptr<widget> subwidget, bool disable)
        : subwidget(std::move(subwidget))
        , disable(disable)
    {
    }

    item_result disabler::draw_impl()
    {
        if (auto _ = disabled(disable))
            return subwidget->draw();
        else
            return {};
    }

    sequence::sequence(std::vector<std::unique_ptr<widget>> subwidgets)
        : subwidgets(std::move(subwidgets))
    {
    }

    item_result sequence::draw_impl()
    {
        item_result result;
        for (auto& subwidget : subwidgets)
            result |= subwidget->draw();
        return result;
    }
}
