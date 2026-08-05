#pragma once

#include "imtk/instance_guard.hpp"
#include "imtk/util.hpp"

#include <imgui.h>

#include <functional>
#include <optional>
#include <typeindex>

namespace imtk
{
	class drag_drop_type
	{
		std::type_index _ti;

	public:
		drag_drop_type(std::type_index ti);

		std::string repr() const;
		operator bool() const;
	};

	template<typename ty>
	drag_drop_type make_drag_drop_type()
	{
		return drag_drop_type(typeid(ty));
	}

	struct drag_droppable
	{
		virtual void send(const std::function<void(const void*, size_t)>& dump) const = 0;
	};
	
	template<std::derived_from<drag_droppable> dd>
	struct drag_drop_convert
	{
		static_assert(deferred_false<dd>);

		using payload_view = void;
		payload_view view(const void* buf, size_t size) const {}
	};

	template<std::derived_from<drag_droppable> dd>
	void send_drag_drop_payload(dd obj, ImGuiCond cond = 0)
	{
		obj.send([cond](const void* buf, size_t size) { ImGui::SetDragDropPayload(make_drag_drop_type<dd>().repr().c_str(), buf, size, cond); });
	}

	template<std::derived_from<drag_droppable> dd>
	bool drag_drop_is_type()
	{
		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		return payload && payload->IsDataType(make_drag_drop_type<dd>().repr().c_str());
	}

	template<std::derived_from<drag_droppable> dd>
	std::optional<typename drag_drop_convert<dd>::payload_view> drag_drop_get()
	{
		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (payload && payload->IsDataType(make_drag_drop_type<dd>().repr().c_str()))
			return drag_drop_convert<dd>{}.view(payload->Data, payload->DataSize);
		else
			return std::nullopt;
	}

	template<typename derived>
	struct drag_droppable_pod : public drag_droppable
	{
		void send(const std::function<void(const void*, size_t)>& dump) const override
		{
			dump(this, sizeof(derived));
		}
	};

	template<typename derived> requires std::derived_from<derived, drag_droppable_pod<derived>>
	struct drag_drop_convert<derived>
	{
		struct pv
		{
			const derived* payload;

			const derived& operator*() const
			{
				return *payload;
			}

			const derived* operator->() const
			{
				return payload;
			}
		};

		using payload_view = pv;

		payload_view view(const void* buf, size_t size) const
		{
			return pv(static_cast<const derived*>(buf));
		}
	};

	class drag_drop_source : public instance_guard<drag_drop_source>
	{
		bool _alive;

	public:
		drag_drop_source(ImGuiDragDropFlags flags = 0);
		drag_drop_source(const drag_drop_source&) = delete;
		drag_drop_source(drag_drop_source&&) noexcept;
		~drag_drop_source();
		drag_drop_source& operator=(drag_drop_source&&) = delete;

		operator bool() const;
	};

	class drag_drop_target : public instance_guard<drag_drop_target>
	{
		bool _alive;

	public:
		drag_drop_target();
		drag_drop_target(const drag_drop_target&) = delete;
		drag_drop_target(drag_drop_target&&) noexcept;
		~drag_drop_target();
		drag_drop_target& operator=(drag_drop_target&&) = delete;

		operator bool() const;

		template<std::derived_from<drag_droppable> dd>
		std::optional<typename drag_drop_convert<dd>::payload_view> accept(ImGuiDragDropFlags flags = 0) const
		{
			// accept() is method of drag_drop_target since a drag-drop target needs to be active when accepting a payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(make_drag_drop_type<dd>().repr().c_str(), flags))
				return drag_drop_convert<dd>{}.view(payload->Data, payload->DataSize);
			else
				return std::nullopt;
		}
	};
}
