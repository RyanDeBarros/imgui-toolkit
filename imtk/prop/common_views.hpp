#pragma once

#include "imtk/edit_session.hpp"
#include "imtk/label_registry.hpp"

#include "imtk/prop/payload.hpp"

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

	template<typename ty>
	struct simple_view<edit_session<ty>> : public iview
	{
		edit_session<ty>& ref;

		simple_view(edit_session<ty>& ref) : ref(ref) {}

		payload dump() const override
		{
			return payload::pod(ref.buffer());
		}

		bool can_load(const payload& pld) const override
		{
			return pld.resolve<ty>();
		}

		bool try_load(const payload& pld) const override
		{
			if (auto data = pld.resolve<ty>())
			{
				if (ref.buffer() != *data)
				{
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

	template<>
	struct simple_view<std::string> : public iview
	{
		std::string& ref;

		simple_view(std::string& ref) : ref(ref) {}

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};

	template<>
	struct simple_view<edit_session<std::string>> : public iview
	{
		edit_session<std::string>& ref;

		simple_view(edit_session<std::string>& ref) : ref(ref) {}

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};

	struct readonly_text_view : public iview
	{
		std::string text;

		readonly_text_view(std::string text) : text(std::move(text)) {}

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};

	struct readonly_view : public iview
	{
		std::unique_ptr<iview> view;

		readonly_view(std::unique_ptr<iview> view) : view(std::move(view)) {}

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

	struct dynamic_combo_view : public iview
	{
		int& index;
		std::vector<std::string> items;

		dynamic_combo_view(int& index, std::vector<std::string> items);

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};
}
