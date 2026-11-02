#pragma once

#include "imtk/label_registry.hpp"

#include "imtk/w/simple_widget.hpp"
#include "imtk/w/widget.hpp"

#include <imgui.h>

#include <functional>
#include <string>
#include <string_view>

namespace imtk::w
{
    struct generic_widget
        : public widget
    {
        std::function<item_result()> draw_fn;

        generic_widget(std::function<item_result()> draw_fn);

    protected:
        item_result draw_impl() override;
    };

    struct bound_optional
        : public widget
    {
        bound_widget<bool> enable;
        std::unique_ptr<widget> item;

        bound_optional(std::unique_ptr<widget>&& item, bool& enable, bound_widget<bool>::config_impl config = {});

    protected:
        item_result draw_impl() override;
    };

    struct simple_optional
        : public widget
    {
        simple_widget<bool> enable;
        std::unique_ptr<widget> item;

        simple_optional(std::unique_ptr<widget>&& item, bool& enable, simple_widget<bool>::config_impl config = {});

    protected:
        item_result draw_impl() override;
    };

    struct combo_widget
        : public widget
    {
        int& index;
        label_span_handle names;

        struct config_impl
        {
            label_handle label;
        } config;

        combo_widget(int& index, label_span_handle names, config_impl config = {});

    protected:
        item_result draw_impl() override;
    };

    struct readonly_text
        : public widget
    {
        std::string& text;

        struct config_impl
        {
            label_handle label;

            ImGuiInputTextFlags flags = 0;
        } config;

        readonly_text(std::string& text, config_impl config = {});

    protected:
        item_result draw_impl() override;
    };

    struct readonly_text_owned
        : public widget
    {
        std::string text;
        readonly_text view;

        readonly_text_owned(std::string text, readonly_text::config_impl config = {});

    protected:
        item_result draw_impl() override;
    };

    struct disabler
        : public widget
    {
        bool disable = false;
        std::unique_ptr<widget> subwidget;

        disabler(std::unique_ptr<widget> subwidget, bool disable);

    protected:
        item_result draw_impl() override;
    };

    struct sequence : public widget
    {
        std::vector<std::unique_ptr<widget>> subwidgets;

        sequence(std::vector<std::unique_ptr<widget>> subwidgets);

    protected:
        item_result draw_impl() override;
    };
}
