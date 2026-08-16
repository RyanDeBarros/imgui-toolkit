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
			return payload::pod(ref);
		}

		bool can_load(const payload& pld) const override
		{
			return pld.resolve<ty>();
		}

		bool try_load(const payload& pld) const override
		{
			if (auto data = pld.resolve<ty>())
			{
				if (ref != *data)
				{
					ref = *data;
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
	};

	template<>
	struct simple_view<std::string> : public iview
	{
		std::string& ref;

		simple_view(std::string& ref) : ref(ref) {}

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
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
