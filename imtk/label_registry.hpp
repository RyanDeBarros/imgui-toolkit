#pragma once

#include <imp/internship.hpp>

#include <span>
#include <string_view>

namespace imtk
{
	struct label_registry
	{
		using data_structure = imp::internship<std::string>;
		using handle = data_structure::handle;

		static handle intern(const std::string_view label);
		static const char* string(const handle handle);
	};

	class label_handle
	{
		label_registry::handle _h;

	public:
		label_handle() = default;
		label_handle(const char* label);
		label_handle(const std::string_view label);
		label_handle& operator=(const char* label);
		label_handle& operator=(const std::string_view label);

		const char* c_str() const;
		operator bool() const;

		bool operator==(const label_handle&) const = default;
		bool operator!=(const label_handle&) const = default;

		size_t hash() const;
	};

	struct label_span_registry
	{
		using data_structure = imp::internship<std::vector<label_handle>>;
		using handle = data_structure::handle;

		static handle intern(const std::vector<label_handle>& labels);
		static handle intern(const std::span<label_handle> labels);
		static label_handle sublabel(const handle handle, size_t i);
		static size_t count(const handle handle);

		static const char* combo_getter(void* user_data, int idx);
	};

	class label_span_handle
	{
		label_span_registry::handle _h;

	public:
		label_span_handle() = default;
		label_span_handle(const std::vector<label_handle>& labels);
		label_span_handle(const std::span<label_handle> labels);
		label_span_handle(const char** labels, size_t count);

		label_handle sublabel(size_t i) const;
		size_t count() const;

		bool operator==(const label_span_handle&) const = default;
		bool operator!=(const label_span_handle&) const = default;
	};
}

template<>
struct std::hash<imtk::label_handle>
{
	size_t operator()(const imtk::label_handle& h) const
	{
		return h.hash();
	}
};
