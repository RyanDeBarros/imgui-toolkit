#pragma once

#include "imtk/prop/payload.hpp"

namespace imtk::prop
{
	template<>
	struct payload_interface<std::string>
	{
		static payload dump(const std::string& value);
		static bool can_load(const payload& pld);
		static std::optional<std::string> load(const payload& pld);
	};
}
