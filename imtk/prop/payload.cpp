#include "payload.hpp"

namespace imtk::prop
{
	payload::payload(const void* data, size_t size, imp::type_erasure type)
		: type(type)
	{
		this->data.resize(size);
		std::memcpy(this->data.data(), data, size);
	}

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
}
