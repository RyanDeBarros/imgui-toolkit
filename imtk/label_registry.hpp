#pragma once

#include <imp/internship.hpp>

#include <span>
#include <string_view>

namespace imtk
{
	struct label_registry
	{
		using data_structure = imp::internship<std::string>;
		using handle = data_structure::handle_type;

		static handle intern(const std::string_view label);
		static const char* string(const handle handle);
	};

	struct label_span_registry
	{
		using data_structure = imp::internship<std::vector<std::string>>;
		using handle = data_structure::handle_type;

		static handle intern(const std::vector<std::string>& labels);
		static handle intern(const std::span<std::string_view> labels);
		static handle intern(const std::span<const char* const> labels);
		static const char* string(const handle handle, size_t i);
		static size_t count(const handle handle);

		static const char* combo_getter(void* user_data, int idx);
	};
}
