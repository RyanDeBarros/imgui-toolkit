#pragma once

#include "imtk/prop/payload.hpp"
#include "imtk/label_registry.hpp"

namespace imtk::prop
{
	struct view_list : public iview
	{
		std::vector<std::unique_ptr<iview>> subviews;

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};

	template<typename ty>
	struct simple_view : public iview
	{
		ty& ref;

		simple_view(ty& ref) : ref(ref) {}

		payload dump() const override
		{
			return payload_interface<ty>::dump(ref);
		}

		bool can_load(const payload& pld) const override
		{
			return payload_interface<ty>::can_load(pld);
		}

		bool try_load(const payload& pld) const override
		{
			auto obj = payload_interface<ty>::load(pld);
			if (obj && ref != *obj)
			{
				ref = *obj;
				return true;
			}
			else
				return false;
		}
	};

	struct combo_view : public iview
	{
		int& index;
		label_span_registry::handle names;

		combo_view(int& index, label_span_registry::handle names);

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};
}
