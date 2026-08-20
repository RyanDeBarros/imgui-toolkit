#pragma once

#include "imtk/simple_scopes.hpp"

#include <memory>

namespace imtk
{
	class collapsing_section
	{
		bool _valid = true;
		bool _visible = false;
		std::unique_ptr<child> _child;

	public:
		collapsing_section(std::string_view label, bool start_open = false);
		collapsing_section(const collapsing_section&) = delete;
		collapsing_section(collapsing_section&&) noexcept;
		~collapsing_section();
		collapsing_section& operator=(collapsing_section&&) noexcept = delete;

		operator bool() const;
	};
}
