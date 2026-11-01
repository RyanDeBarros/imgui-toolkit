#include "form.hpp"

#include "imtk/errors.hpp"
#include "imtk/simple_scopes.hpp"

#include "imtk/prop/property_grid.hpp"

#include <memory>

namespace imtk::prop
{
    form* active_form = nullptr;
    std::unique_ptr<child> form_child;
    std::unique_ptr<table> form_table;

    form::form()
    {
        begin_table();
    }

    form::form(form&& o) noexcept
        : draw_content_(o.draw_content_)
        , id_counter_(o.id_counter_)
        , scope_(std::move(o.scope_))
    {
        o.draw_content_ = false;
        o.id_counter_ = 0;
        o.valid_ = false;

        if (&o == active_form)
            active_form = this;
    }

    form::~form()
    {
        if (valid_)
            end_table();
    }

    form* form::active_instance()
    {
        return active_form;
    }

    form::operator bool() const
    {
        return valid_ && draw_content_;
    }

    void form::begin_table()
    {
        if (active_form)
            throw error(error_code::existing_active_instance);

        active_form = this;
        scope_.push(&active_form).push(id_counter_++);

        draw_content_ = false;
        form_table.reset();
        form_child = std::make_unique<child>("##imtk-formID", ImVec2(0.f, 0.f), ImGuiChildFlags_AutoResizeY);
        if (*form_child)
        {
            form_table = std::make_unique<table>(
                "",
                3,
                ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingFixedFit);

            if (*form_table)
            {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFrameHeight());
                draw_content_ = true;
            }
        }
    }

    void form::end_table()
    {
        form_table.reset();
        form_child.reset();
        draw_content_ = false;

        scope_.pop_all();
        active_form = nullptr;
    }

    // TODO maybe instead of checking in_form(), do away with scopes and call
    // imtk::prop::form as a function that takes a callback (that executes on
    // draw_content_ = true). Then in that callback, can call imtk::prop::form_pause()
    // which has two callbacks - on for 'inside' the pause, and one for after resuming the pause.
    bool in_form()
    {
        return active_form && *active_form;
    }

    form::pause::pause(bool resume_after)
        : form_(active_form)
        , resume_after_(resume_after)
    {
        if (form_)
            was_drawing_content_ = form_->draw_content_;

        if (form_)
            form_->end_table();
    }

    form::pause::~pause()
    {
        if (form_ && resume_after_)
            form_->begin_table();
    }

    form::pause::operator bool() const
    {
        return form_ && form_->valid_ && was_drawing_content_;
    }

    // TODO DEBT use generators for as many headers as possible
    subform::subform(std::string_view label, const view_generator& property_generator, config cfg)
        : pause_(cfg.resume_after)
        , section_(label, cfg.start_open)
    {
        grid::check_header(property_generator);

        if (section_)
            subform_.emplace();
    }

    subform::subform(std::string_view label, config cfg)
        : pause_(cfg.resume_after), section_(label, cfg.start_open)
    {
        if (section_)
            subform_.emplace();
    }

    subform::operator bool() const
    {
        return pause_ && section_ && subform_ && *subform_;
    }
}
