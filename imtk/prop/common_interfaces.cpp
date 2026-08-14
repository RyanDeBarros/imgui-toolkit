#include "common_interfaces.hpp"

namespace imtk::prop
{
	payload payload_interface<std::string>::dump(const std::string& value)
	{
		return payload(value.data(), value.size(), imp::erase_type<std::string>());
	}

	bool payload_interface<std::string>::can_load(const payload& pld)
	{
		return pld.type == imp::erase_type<std::string>();
	}

	std::optional<std::string> payload_interface<std::string>::load(const payload& pld)
	{
		if (pld.type == imp::erase_type<std::string>())
			return std::string(reinterpret_cast<const char*>(pld.data.data()), pld.data.size());
		else
			return std::nullopt;
	}
}
