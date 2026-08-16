#pragma once

#include "imtk/collapsing_section.hpp"

#include "imtk/prop/form.hpp"
#include "imtk/prop/payload.hpp"

namespace imtk::prop
{
	class subform
	{
		form_pause _pause;
		collapsing_section _section;
		std::optional<form> _subform;

	public:
		subform(const char* label, const view_generator& property_generator, bool start_open = false);
		subform(const char* label, bool start_open = false);
		subform(const subform&) = delete;
		subform(subform&&) = delete;

		operator bool() const;
	};
}
