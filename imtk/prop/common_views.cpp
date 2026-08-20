#include "common_views.hpp"

namespace imtk::prop
{
	struct payload_list
	{
		// TODO DO NOT USE HEAP ALLOCATION FOR PAYLOADS. Make a note of this in documentations
		std::vector<imp::box> payloads;
	};

	imp::box view_list::dump() const
	{
		auto list = new payload_list();
		for (const auto& view : subviews)
			list->payloads.push_back(view->dump());
		return imp::box(list);
	}

	bool view_list::can_load(const imp::box& pld) const
	{
		if (auto list = pld.as<payload_list>())
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

	bool view_list::try_load(const imp::box& pld) const
	{
		if (auto list = pld.as<payload_list>())
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

	struct readonly_text_payload
	{
		std::string text;
	};

	imp::box readonly_text_view::dump() const
	{
		return imp::make_box<readonly_text_payload>(text);
	}

	bool readonly_text_view::can_load(const imp::box& pld) const
	{
		return false;
	}

	bool readonly_text_view::try_load(const imp::box& pld) const
	{
		return false;
	}

	imp::box readonly_view::dump() const
	{
		return view->dump();
	}

	bool readonly_view::can_load(const imp::box&) const
	{
		return false;
	}
	
	bool readonly_view::try_load(const imp::box&) const
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

	imp::box combo_view::dump() const
	{
		return imp::make_box<combo_payload>(index, names);
	}

	bool combo_view::can_load(const imp::box& pld) const
	{
		if (auto data = pld.as<combo_payload>())
			return data->names == names;
		else
			return false;
	}

	bool combo_view::try_load(const imp::box& pld) const
	{
		if (auto data = pld.as<combo_payload>())
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

	struct dynamic_combo_payload
	{
		std::string item;
	};

	imp::box dynamic_combo_view::dump() const
	{
		return imp::make_box<dynamic_combo_payload>(items[index]);
	}
	
	bool dynamic_combo_view::can_load(const imp::box& pld) const
	{
		if (auto data = pld.as<dynamic_combo_payload>())
		{
			for (auto it = items.begin(); it != items.end(); ++it)
			{
				if (*it == data->item)
					return true;
			}

			return false;
		}
		else
			return false;
	}
	
	bool dynamic_combo_view::try_load(const imp::box& pld) const
	{
		if (auto data = pld.as<dynamic_combo_payload>())
		{
			if (items[index] == data->item)
				return false;

			for (auto it = items.begin(); it != items.end(); ++it)
			{
				if (*it == data->item)
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
