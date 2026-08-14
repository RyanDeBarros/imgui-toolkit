#pragma once

#include "imtk/desc/datapath.hpp"

#include <unordered_map>
#include <variant>

namespace imtk::desc
{
	template<typename d>
	d clone_data(const d& desc)
	{
		d copy{};
		copy.copy_data(desc);
		return copy;
	}

	template<typename d>
	class vector
	{
		std::vector<d> _vector;

	public:
		datapath_link link;

		vector(datapath_link link)
			: link(std::move(link))
		{
		}

		d& push_back()
		{
			_vector.push_back(d(datapath_link(link, datapath::step(_vector.size()))));
			return _vector.back();
		}

		void insert(size_t i, d element)
		{
			for (auto it = _vector.begin() + i; it != _vector.end(); ++it)
				it->link.set_step(*it->link.step() + 1);

			element.link = datapath_link(link, datapath::step(i));
			_vector.insert(_vector.begin() + i, std::move(element));
		}

		void remove(size_t i)
		{
			_vector.erase(_vector.begin() + i);
			for (auto it = _vector.begin() + i; it != _vector.end(); ++it)
				it->link.set_step(*it->link.step() - 1);
		}

		void clear()
		{
			_vector.clear();
		}

		size_t size() const
		{
			return _vector.size();
		}

		void resize(size_t new_size)
		{
			if (new_size < _vector.size())
				_vector.erase(_vector.begin() + new_size, _vector.end());
			else if (new_size > _vector.size())
			{
				for (size_t i = _vector.size(); i < new_size; ++i)
					_vector.push_back(d(datapath_link(link, datapath::step(i))));
			}
		}

		bool empty() const
		{
			return _vector.empty();
		}

		d& back()
		{
			return _vector.back();
		}

		const d& back() const
		{
			return _vector.back();
		}

		d& operator[](size_t i)
		{
			return _vector[i];
		}

		const d& operator[](size_t i) const
		{
			return _vector[i];
		}

		auto begin() const
		{
			return _vector.begin();
		}

		auto begin()
		{
			return _vector.begin();
		}

		auto end() const
		{
			return _vector.end();
		}

		auto end()
		{
			return _vector.end();
		}

		void* resolve(datapath_view path, std::type_index type)
		{
			if (path.empty())
				return typeid(decltype(*this)) == type ? static_cast<void*>(this) : nullptr;

			int index = path.step();
			if (index >= 0 && index < _vector.size())
				return _vector[index].resolve(path.next(), type);
			else
				return nullptr;
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			if (path.empty())
				os << "<error>";
			else
			{
				int index = path.step();
				if (index >= 0 && index < _vector.size())
				{
					path = path.next();
					if (path.empty())
						os << index;
					else
					{
						os << index << ".";
						_vector[index].describe(os, path);
					}
				}
				else
					os << "<error>";
			}
		}

		bool query_dirty(const vector<d>& disk) const
		{
			if (_vector.size() != disk._vector.size())
				return true;

			for (size_t i = 0; i < _vector.size(); ++i)
			{
				if (_vector[i].query_dirty(disk._vector[i]))
					return true;
			}

			return false;
		}

		void copy_data(const vector<d>& o)
		{
			resize(o.size());
			for (size_t i = 0; i < _vector.size(); ++i)
				_vector[i].copy_data(o._vector[i]);
		}
	};

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

		void* resolve(datapath_view path, std::type_index type)
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

	template<typename key, typename d>
	class map
	{
		std::unordered_map<key, d> _map;

	public:
		datapath_link link;

		map(datapath_link link)
			: link(std::move(link))
		{
		}

		void clear()
		{
			_map.clear();
		}

		d& operator[](key key)
		{
			auto it = _map.find(key);
			if (it != _map.end())
				return it->second;
			else
				return _map.emplace(key, datapath_link(link, datapath::step(key))).first->second;
		}

		auto begin()
		{
			return _map.begin();
		}

		auto end()
		{
			return _map.end();
		}

		void* resolve(datapath_view path, std::type_index type)
		{
			if (path.empty())
				return typeid(decltype(*this)) == type ? static_cast<void*>(this) : nullptr;

			auto it = _map.find(static_cast<key>((int)path.step()));
			if (it != _map.end())
				return it->second.resolve(path.next(), type);
			else
				return nullptr;
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			if (path.empty())
				os << "<error>";
			else
			{
				auto k = static_cast<key>((int)path.step());
				auto it = _map.find(k);
				if (it != _map.end())
				{
					path = path.next();
					if (path.empty())
						os << k;
					else
					{
						os << k << ".";
						it->second.describe(os, path);
					}
				}
				else
					os << "<error>";
			}
		}

		bool query_dirty(const map<key, d>& disk) const
		{
			if (_map.size() != disk._map.size())
				return true;

			for (const auto& [key, desc] : _map)
			{
				auto it = disk._map.find(key);
				if (it == disk._map.end())
					return true;

				if (desc.query_dirty(it->second))
					return true;
			}

			return false;
		}

		void copy_data(const map<key, d>& o)
		{
			for (auto it = _map.begin(); it != _map.end(); ++it)
			{
				auto o_it = o._map.find(it->first);
				if (o_it != o._map.end())
					it->second.copy_data(o_it->second);
				else
					it = _map.erase(it);
			}

			for (const auto& [o_key, o_desc] : o._map)
			{
				if (!_map.contains(o_key))
					(*this)[o_key].copy_data(o_desc);
			}
		}
	};
}
