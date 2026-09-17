#include "label_registry.hpp"

#include <imp/hash.hpp>

#include <algorithm>

namespace imtk
{
	static label_registry::data_structure label_registry_instance;
	static label_span_registry::data_structure label_span_registry_instance;

	label_registry::handle label_registry::intern(const std::string_view label)
	{
		return !label.empty() ? label_registry_instance.intern(label) : label_registry::handle();
	}

	const char* label_registry::string(const handle handle)
	{
		if (auto ptr = label_registry_instance.try_get(handle))
			return ptr->c_str();
		else
			return "";
	}
	
	label_handle::label_handle(const std::string_view label)
		: _h(label_registry::intern(label))
	{
	}

	label_handle::label_handle(const char* label)
		: _h(label_registry::intern(label))
	{
	}

	label_handle& label_handle::operator=(const std::string_view label)
	{
		_h = label_registry::intern(label);
		return *this;
	}

	label_handle& label_handle::operator=(const char* label)
	{
		_h = label_registry::intern(label);
		return *this;
	}

	const char* label_handle::c_str() const
	{
		return label_registry::string(_h);
	}

	label_handle::operator bool() const
	{
		return _h.valid();
	}

	size_t label_handle::hash() const
	{
		return std::hash<label_registry::handle>{}(_h);
	}

	struct label_span_helper
	{
		struct hash
		{
			size_t operator()(const std::vector<label_handle>& vec) const
			{
				imp::hasher h;
				for (const auto& s : vec)
					h.with(s);
				return h;
			}

			size_t operator()(const std::span<label_handle> span) const
			{
				imp::hasher h;
				for (const auto& s : span)
					h.with(s);
				return h;
			}
		};

		struct equal
		{
			bool operator()(const std::vector<label_handle> a, const std::span<label_handle>& b) const
			{
				return std::ranges::equal(a, b);
			}
		};

		struct converter
		{
			std::vector<label_handle> operator()(const std::span<label_handle> span) const
			{
				return std::vector(span.begin(), span.end());
			}
		};
	};

    label_span_registry::handle label_span_registry::intern(const std::vector<label_handle>& labels)
    {
        return !labels.empty()
            ? label_span_registry_instance.intern<decltype(labels), label_span_helper::hash>(labels)
            : label_span_registry::handle();
    }

	label_span_registry::handle label_span_registry::intern(const std::span<label_handle> labels)
	{
		return !labels.empty()
			? label_span_registry_instance.intern<decltype(labels), label_span_helper::hash, label_span_helper::equal, label_span_helper::converter>(labels)
			: label_span_registry::handle();
	}

	label_handle label_span_registry::sublabel(const handle handle, size_t i)
    {
		if (auto ptr = label_span_registry_instance.try_get(handle))
			return (*ptr)[i];
		else
			return {};
    }

	size_t label_span_registry::count(const handle handle)
	{
		if (auto ptr = label_span_registry_instance.try_get(handle))
			return ptr->size();
		else
			return 0;
	}

	const char* label_span_registry::combo_getter(void* user_data, int idx)
	{
		return sublabel(*static_cast<handle*>(user_data), idx).c_str();
	}

	label_span_handle::label_span_handle(const std::vector<label_handle>& labels)
		: _h(label_span_registry::intern(labels))
	{
	}

	label_span_handle::label_span_handle(const std::span<label_handle> labels)
		: _h(label_span_registry::intern(labels))
	{
	}

	label_span_handle::label_span_handle(const char** labels, size_t count)
	{
		std::vector<label_handle> handles;
		handles.reserve(count);
		for (size_t i = 0; i < count; ++i)
			handles.emplace_back(labels[i]);
		_h = label_span_registry::intern(handles);
	}

	label_handle label_span_handle::sublabel(size_t i) const
	{
		return label_span_registry::sublabel(_h, i);
	}

	size_t label_span_handle::count() const
	{
		return label_span_registry::count(_h);
	}
}
