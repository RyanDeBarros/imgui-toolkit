#include "common_views.hpp"

namespace imtk::prop
{
	struct payload_list
	{
		std::vector<payload> payloads;
	};

	payload view_list::dump() const
	{
		payload_list list;
		for (const auto& view : subviews)
			list.payloads.push_back(view->dump());
		return payload::pod(list);
	}

	bool view_list::can_load(const payload& pld) const
	{
		if (auto list = pld.resolve<payload_list>())
		{
			if (list->payloads.size() != subviews.size())
				return false;

			size_t i = 0;
			for (const auto& view : subviews)
			{
				if (!view->can_load(list->payloads[i++]))
					return false;
			}

			return true;
		}
		else
			return false;
	}

	bool view_list::try_load(const payload& pld) const
	{
		if (auto list = pld.resolve<payload_list>())
		{
			if (list->payloads.size() != subviews.size())
				return false;

			size_t i = 0;
			for (const auto& view : subviews)
			{
				if (!view->try_load(list->payloads[i++]))
					return false;
			}

			return true;
		}
		else
			return false;
	}

	payload simple_view<std::string>::dump() const
	{
		return payload(ref.data(), ref.size(), imp::erase_type<std::string>());
	}

	bool simple_view<std::string>::can_load(const payload& pld) const
	{
		return pld.type == imp::erase_type<std::string>();
	}

	bool simple_view<std::string>::try_load(const payload& pld) const
	{
		if (pld.type == imp::erase_type<std::string>())
		{
			std::string_view sv(reinterpret_cast<const char*>(pld.data.data()), pld.data.size());
			if (ref != sv)
			{
				ref = sv;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	payload readonly_view::dump() const
	{
		return view->dump();
	}

	bool readonly_view::can_load(const payload&) const
	{
		return false;
	}
	
	bool readonly_view::try_load(const payload&) const
	{
		return false;
	}

	struct combo_payload
	{
		int index;
		label_span_registry::handle names;
	};

	combo_view::combo_view(int& index, label_span_registry::handle names)
		: index(index), names(names)
	{
	}

	payload combo_view::dump() const
	{
		return payload::pod(combo_payload{ .index = index, .names = names });
	}

	bool combo_view::can_load(const payload& pld) const
	{
		if (auto data = pld.resolve<combo_payload>())
			return data->names == names;
		else
			return false;
	}

	bool combo_view::try_load(const payload& pld) const
	{
		if (auto data = pld.resolve<combo_payload>())
		{
			if (data->names == names && index != data->index)
			{
				index = data->index;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	dynamic_combo_view::dynamic_combo_view(int& index, std::vector<std::string> items)
		: index(index), items(std::move(items))
	{
	}

	payload dynamic_combo_view::dump() const
	{
		return payload(items[index].data(), items[index].size(), imp::erase_type<dynamic_combo_view>());
	}
	
	bool dynamic_combo_view::can_load(const payload& pld) const
	{
		if (pld.resolve<dynamic_combo_view>())
		{
			std::string_view sv(reinterpret_cast<const char*>(pld.data.data()), pld.data.size());
			for (auto it = items.begin(); it != items.end(); ++it)
			{
				if (*it == sv)
					return true;
			}

			return false;
		}
		else
			return false;
	}
	
	bool dynamic_combo_view::try_load(const payload& pld) const
	{
		if (pld.resolve<dynamic_combo_view>())
		{
			std::string_view sv(reinterpret_cast<const char*>(pld.data.data()), pld.data.size());
			
			if (items[index] == sv)
				return false;

			for (auto it = items.begin(); it != items.end(); ++it)
			{
				if (*it == sv)
				{
					index = it - items.begin();
					return true;
				}
			}

			return false;
		}
		else
			return false;
	}
}
