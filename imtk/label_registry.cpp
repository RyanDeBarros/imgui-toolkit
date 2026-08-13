#include "label_registry.hpp"

#include <imp/hash.hpp>

#include <algorithm>

namespace imtk
{
	static label_registry::data_structure label_registry_instance;
	static label_span_registry::data_structure label_span_registry_instance;

	label_registry::handle label_registry::intern(const std::string_view label)
	{
		return label_registry_instance.intern(label);
	}

	const char* label_registry::string(const handle handle)
	{
		return label_registry_instance.get(handle).c_str();
	}

	struct label_span_helper
	{
		size_t operator()(const std::vector<std::string>& span) const
		{
			imp::hasher h;
			for (const auto& s : span)
				h.with(s);
			return h;
		}

		size_t operator()(const std::span<std::string_view> span) const
		{
			imp::hasher h;
			for (const auto& s : span)
				h.with(s);
			return h;
		}

		size_t operator()(const std::span<const char* const> span) const
		{
			imp::hasher h;
			for (const auto& s : span)
				h.with(std::string_view(s));
			return h;
		}

		bool operator()(const std::vector<std::string>& a, const std::span<std::string_view>& b) const
		{
			return std::ranges::equal(a, b);
		}

		bool operator()(const std::vector<std::string>& a, const std::span<const char* const>& b) const
		{
			return std::ranges::equal(a, b);
		}
	};

	struct label_span_conversion
	{
		std::vector<std::string> operator()(const std::span<std::string_view>& span) const
		{
			std::vector<std::string> v;
			v.reserve(span.size());
			for (const auto& s : span)
				v.push_back(std::string(s));
			return v;
		}

		std::vector<std::string> operator()(const std::span<const char* const>& span) const
		{
			std::vector<std::string> v;
			v.reserve(span.size());
			for (const auto& s : span)
				v.push_back(std::string(s));
			return v;
		}
	};

	label_span_registry::handle label_span_registry::intern(const std::vector<std::string>& labels)
	{
		return label_span_registry_instance.intern<decltype(labels), label_span_helper>(labels);
	}

	label_span_registry::handle label_span_registry::intern(const std::span<std::string_view> labels)
	{
		return label_span_registry_instance.intern<decltype(labels), label_span_helper, label_span_helper, label_span_conversion>(labels);
	}

	label_span_registry::handle label_span_registry::intern(const std::span<const char* const> labels)
	{
		return label_span_registry_instance.intern<decltype(labels), label_span_helper, label_span_helper, label_span_conversion>(labels);
	}

	const char* label_span_registry::string(const handle handle, size_t i)
	{
		return label_span_registry_instance.get(handle)[i].c_str();
	}

	size_t label_span_registry::count(const handle handle)
	{
		return label_span_registry_instance.get(handle).size();
	}

	const char* label_span_registry::combo_getter(void* user_data, int idx)
	{
		return string(*static_cast<handle*>(user_data), idx);
	}
}
