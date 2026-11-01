#pragma once

#include "imtk/collapsing_section.hpp"
#include "imtk/id_scope.hpp"

#include "imtk/prop/view.hpp"

#include <optional>

namespace imtk::prop
{
    class form
    {
        bool valid_ = true;
        bool draw_content_ = false;
        int id_counter_ = 0;
        id_scope scope_;

    public:
        form();
        form(const form&) = delete;
        form(form&&) noexcept;
        ~form();
        form& operator=(form&&) noexcept = delete;

        static form* active_instance();

        operator bool() const;

    private:
        void begin_table();
        void end_table();

    public:
        // TODO LATER add important library documentation: After pausing a form, make sure to check
        // imtk::prop::in_form() before continuing to draw properties. Likewise after subforms end.
        // Basically whenever a property will be drawn.
        class pause
        {
            form* form_ = nullptr;
            bool was_drawing_content_ = false;
            bool resume_after_;

        public:
            pause(bool resume_after = true);
            pause(const pause&) = delete;
            pause(pause&&) = delete;
            ~pause();

            operator bool() const;
        };
    };

    class subform
    {
        form::pause pause_;
        collapsing_section section_;
        std::optional<form> subform_;

    public:
        struct config
        {
            bool resume_after = true;
            bool start_open = false;
        };

        subform(std::string_view label, const view_generator& property_generator, config cfg = {});
        subform(std::string_view label, config cfg = {});
        subform(const subform&) = delete;
        subform(subform&&) = delete;

        operator bool() const;
    };

    extern bool in_form();
}
