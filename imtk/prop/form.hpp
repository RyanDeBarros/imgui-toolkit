#pragma once

#include "imtk/collapsing_section.hpp"
#include "imtk/id_scope.hpp"

#include "imtk/prop/payload.hpp"

namespace imtk::prop
{
	class form
	{
		bool _valid = true;
		bool _draw_content = false;
		int _id_counter = 0;
		id_scope _scope;

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
		// TODO add important library documentation: After pausing a form, make sure to check imtk::prop::in_form() before continuing to draw properties. Likewise for subforms
		class pause
		{
			form* _form = nullptr;
			bool _was_drawing_content = false;
			bool _resume_after;

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
		form::pause _pause;
		collapsing_section _section;
		std::optional<form> _subform;

	public:
		struct config
		{
			bool resume_after = true;
			bool start_open = false;
		};

		subform(const char* label, const view_generator& property_generator, config cfg = {});
		subform(const char* label, config cfg = {});
		subform(const subform&) = delete;
		subform(subform&&) = delete;

		operator bool() const;
	};

	extern bool in_form();
}
