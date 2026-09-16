#pragma once

#include "imtk/datapath.hpp"
#include "imtk/key.hpp"

#include <variant>

namespace imtk::desc
{
	template<typename... ds>
	class variant
	{
	public:
		datapath_link link;

	private:
		std::variant<ds...> _variant;

	public:
		variant(datapath_link link)
			: link(std::move(link)), _variant(std::in_place_index<0>, this->link.share())
		{
		}

		template<typename d>
		variant(datapath_link link, d&& val)
			: link(std::move(link)), _variant(std::forward<d>(val))
		{
			std::visit([this](auto& v) { v.link = this->link.share(); }, _variant);
		}

		template<typename d>
		d& set()
		{
			_variant = d(link.share());
			return std::get<d>(_variant);
		}

		template<typename d>
		void set(d&& desc)
		{
			_variant = std::forward<d>(desc);
			std::visit([this](auto& v) { v.link = link.share(); }, _variant);
		}

		auto visit(auto&& visitor)
		{
			return std::visit([&visitor](auto& desc) { return visitor(desc); }, _variant);
		}

		auto visit(auto&& visitor) const
		{
			return std::visit([&visitor](const auto& desc) { return visitor(desc); }, _variant);
		}

		template<typename d>
		d* try_get()
		{
			return std::get_if<d>(&_variant);
		}

		template<typename d>
		const d* try_get() const
		{
			return std::get_if<d>(&_variant);
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return std::visit([path, type](auto& desc) { return desc.resolve(path, type); }, _variant);
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			return std::visit([&os, path](auto& desc) { return desc.describe(os, path); }, _variant);
		}

		bool query_dirty(const variant<ds...>& disk) const
		{
			return std::visit([](const auto& lhs, const auto& rhs) {
				using lty = std::decay_t<decltype(lhs)>;
				using rty = std::decay_t<decltype(rhs)>;

				if constexpr (std::is_same_v<lty, rty>)
					return lhs.query_dirty(rhs);
				else
					return true;
				}, _variant, disk._variant);
		}

		void copy_data(const variant<ds...>& o)
		{
			std::visit([this](auto& lhs, const auto& rhs) {
				using lty = std::decay_t<decltype(lhs)>;
				using rty = std::decay_t<decltype(rhs)>;

				if constexpr (std::is_same_v<lty, rty>)
					lhs.copy_data(rhs);
				else
					set<rty>().copy_data(rhs);
				}, _variant, o._variant);
		}
	};

	template<typename... ds>
	class subvariant
	{
	public:
		datapath_link link;
		key key_;

	private:
		std::variant<ds...> _variant;

	public:
		subvariant(key key, datapath_link link)
			: link(std::move(link)), key_(key), _variant(std::in_place_index<0>, key, this->link.share())
		{
		}

		template<typename d>
		subvariant(key key_, datapath_link link, d&& val)
			: link(std::move(link)), key_(key), _variant(std::forward<d>(val))
		{
			std::visit([this](auto& v) { v.link = this->link.share(); v.key_ = this->key_; }, _variant);
		}

		template<typename d>
		d& set()
		{
			_variant = d(key_, link.share());
			return std::get<d>(_variant);
		}

		template<typename d>
		void set(d&& desc)
		{
			_variant = std::forward<d>(desc);
			std::visit([this](auto& v) { v.link = link.share(); v.key_ = key_; }, _variant);
		}

		auto visit(const auto& visitor)
		{
			return std::visit([&visitor](auto& desc) { return visitor(desc); }, _variant);
		}

		auto visit(const auto& visitor) const
		{
			return std::visit([&visitor](const auto& desc) { return visitor(desc); }, _variant);
		}

		template<typename d>
		d* try_get()
		{
			return std::get_if<d>(&_variant);
		}

		template<typename d>
		const d* try_get() const
		{
			return std::get_if<d>(&_variant);
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return std::visit([path, type](auto& desc) { return desc.resolve(path, type); }, _variant);
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			return std::visit([&os, path](auto& desc) { return desc.describe(os, path); }, _variant);
		}

		bool query_dirty(const subvariant<ds...>& disk) const
		{
			return std::visit([](const auto& lhs, const auto& rhs) {
				using lty = std::decay_t<decltype(lhs)>;
				using rty = std::decay_t<decltype(rhs)>;

				if constexpr (std::is_same_v<lty, rty>)
					return lhs.query_dirty(rhs);
				else
					return true;
				}, _variant, disk._variant);
		}

		void copy_data(const subvariant<ds...>& o)
		{
			std::visit([this](auto& lhs, const auto& rhs) {
				using lty = std::decay_t<decltype(lhs)>;
				using rty = std::decay_t<decltype(rhs)>;

				if constexpr (std::is_same_v<lty, rty>)
					lhs.copy_data(rhs);
				else
					set<rty>().copy_data(rhs);
				}, _variant, o._variant);
		}

		toml_node subnode(toml_node node) const
		{
			return node[encode_key(key_)];
		}

		template<typename value>
		void dump_into(toml::table& table, value&& value_) const
		{
			table.insert_or_assign(encode_key(key_), std::forward<value>(value_));
		}
	};
}
