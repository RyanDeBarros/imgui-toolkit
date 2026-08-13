#pragma once

#include <imp/type_erasure.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace imtk::prop
{
	struct payload
	{
		std::vector<std::byte> data;
		imp::type_erasure type;

		payload(const void* data, size_t size, imp::type_erasure type);

		template<typename ty>
		static payload pod(const ty& value)
		{
			return payload(&value, sizeof(ty), imp::erase_type<ty>());
		}

		template<typename ty>
		const ty* resolve() const
		{
			return imp::resolve_type<ty>(type, data.data());
		}
	};

	struct iview
	{
		virtual ~iview() = default;
		virtual payload dump() const = 0;
		virtual bool can_load(const payload&) const = 0;
		virtual bool try_load(const payload&) const = 0;
	};

	struct view_list : public iview
	{
		std::vector<std::unique_ptr<iview>> subviews;

		payload dump() const override;
		bool can_load(const payload&) const override;
		bool try_load(const payload&) const override;
	};

	// TODO pass row-by-row / element-by-element generator instead so that the full page doesn't need to be generated for CanPaste() check. Perhaps inherit from iview and define custom implementations of dump/can_load/try_load
	using view_generator = std::function<std::unique_ptr<iview>()>;
}
