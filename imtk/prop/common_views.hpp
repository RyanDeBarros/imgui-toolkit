#pragma once

#include "imtk/edit_session.hpp"
#include "imtk/label_registry.hpp"

#include "imtk/prop/view.hpp"

namespace imtk::prop
{
	struct view_list : public iview
	{
		std::vector<std::unique_ptr<iview>> subviews;

		imp::box dump() const override;
		bool can_load(const imp::box&) const override;
		bool try_load(const imp::box&) const override;
	};

	template<typename ty>
	struct simple_view : public iview
	{
		ty& ref;

		simple_view(ty& ref) : ref(ref) {}

		imp::box dump() const override
		{
			return imp::make_box<ty>(ref);
		}

		bool can_load(const imp::box& pld) const override
		{
			return pld.as<ty>();
		}

		bool try_load(const imp::box& pld) const override
		{
			if (auto data = pld.as<ty>())
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

	template<typename ty>
	struct simple_view<edit_session<ty>> : public iview
	{
		edit_session<ty>& ref;

		simple_view(edit_session<ty>& ref) : ref(ref) {}

		imp::box dump() const override
		{
			return imp::make_box<ty>(ref.buffer());
		}

		bool can_load(const imp::box& pld) const override
		{
			return pld.as<ty>();
		}

		bool try_load(const imp::box& pld) const override
		{
			if (auto data = pld.as<ty>())
			{
				if (ref.buffer() != *data)
				{
					// TODO just set ref.buffer() to value -> then call ref.force_confirm(). In that case, don't even specialize edit_session views - just pass buffer() and if try_load() returns true, call force_confirm() in widgets. Can generically define edit_session widgets in that case instead of specializing for each one.
					ref.publish_reset(*data);
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
	};

	struct readonly_text_view : public iview
	{
		std::string text;

		readonly_text_view(std::string text) : text(std::move(text)) {}

		imp::box dump() const override;
		bool can_load(const imp::box&) const override;
		bool try_load(const imp::box&) const override;
	};

	struct readonly_view : public iview
	{
		std::unique_ptr<iview> view;

		readonly_view(std::unique_ptr<iview> view) : view(std::move(view)) {}

		imp::box dump() const override;
		bool can_load(const imp::box&) const override;
		bool try_load(const imp::box&) const override;
	};

	struct combo_view : public iview
	{
		int& index;
		label_span_registry::handle names;

		combo_view(int& index, label_span_registry::handle names);

		imp::box dump() const override;
		bool can_load(const imp::box&) const override;
		bool try_load(const imp::box&) const override;
	};

	struct dynamic_combo_view : public iview
	{
		int& index;
		std::vector<std::string> items;

		dynamic_combo_view(int& index, std::vector<std::string> items);

		imp::box dump() const override;
		bool can_load(const imp::box&) const override;
		bool try_load(const imp::box&) const override;
	};
}
