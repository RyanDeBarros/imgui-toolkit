#pragma once

#include "imtk/datapath.hpp"
#include "imtk/key.hpp"

#include <unordered_map>

namespace imtk::desc
{
	template<typename key_ty, typename d>
	class map
	{
		std::unordered_map<key_ty, d> _map;

	public:
		datapath_link link;
		key key_;

		map(key key, datapath_link link)
			: link(std::move(link)), key_(key)
		{
		}

		void clear()
		{
			_map.clear();
		}

		d& operator[](key_ty key)
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

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			if (path.empty())
				return imp::resolve_type(type, this);

			auto it = _map.find(static_cast<key_ty>((int)path.step()));
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
				auto k = static_cast<key_ty>((int)path.step());
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

		bool query_dirty(const map<key_ty, d>& disk) const
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

		void copy_data(const map<key_ty, d>& o)
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
