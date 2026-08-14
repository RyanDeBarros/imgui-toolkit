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
}
