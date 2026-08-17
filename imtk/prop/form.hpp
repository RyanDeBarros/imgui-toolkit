#pragma once

#include "imtk/id_scope.hpp"

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
		friend class form_pause;
		void begin_table();
		void end_table();
	};

	extern bool in_form();

	// TODO add important library documentation: After pausing a form, make sure to check imtk::prop::in_form() before continuing to draw properties
	class form_pause
	{
		form* _form = nullptr;
		bool _was_drawing_content = false;

	public:
		form_pause();
		form_pause(const form_pause&) = delete;
		form_pause(form_pause&&) = delete;
		~form_pause();

		operator bool() const;
	};
}
