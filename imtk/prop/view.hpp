#pragma once

#include <imp/box.hpp>

#include <functional>

namespace imtk::prop
{
	struct iview
	{
		virtual ~iview() = default;
		virtual imp::box dump() const = 0;
		virtual bool can_load(const imp::box&) const = 0;
		virtual bool try_load(const imp::box&) const = 0;
	};

	// TODO pass row-by-row / element-by-element generator instead so that the full page doesn't need to be generated for CanPaste() check. Perhaps inherit from iview and define custom implementations of dump/can_load/try_load
	using view_generator = std::function<std::unique_ptr<iview>()>;
}
