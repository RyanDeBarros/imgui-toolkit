#pragma once

#include "imtk/datapath.hpp"
#include "imtk/key.hpp"

namespace imtk::desc
{
	template<typename d, size_t n>
	class array
	{
	public:
		datapath_link link;
		key key_;

	private:
		std::array<d, n> _array;

	public:
		array(key key, datapath_link link)
			: link(std::move(link)), key_(key), _array(init_array(this->link, std::make_index_sequence<n>{}))
		{
		}

	private:
		template<size_t... i>
		static auto init_array(datapath_link& link, std::index_sequence<i...>)
		{
			return std::array<d, n>{ d(datapath_link(link, datapath::step(i)))... };
		}

	public:
		size_t size() const
		{
			return n;
		}

		d& operator[](size_t i)
		{
			return _array[i];
		}

		const d& operator[](size_t i) const
		{
			return _array[i];
		}

		auto begin() const
		{
			return _array.begin();
		}

		auto begin()
		{
			return _array.begin();
		}

		auto end() const
		{
			return _array.end();
		}

		auto end()
		{
			return _array.end();
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			if (path.empty())
				return imp::resolve_type(type, this);

			int index = path.step();
			if (index >= 0 && index < n)
				return _array[index].resolve(path.next(), type);
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
				if (index >= 0 && index < n)
				{
					path = path.next();
					if (path.empty())
						os << index;
					else
					{
						os << index << ".";
						_array[index].describe(os, path);
					}
				}
				else
					os << "<error>";
			}
		}

		bool query_dirty(const array<d, n>& disk) const
		{
			for (size_t i = 0; i < n; ++i)
			{
				if (_array[i].query_dirty(disk._array[i]))
					return true;
			}

			return false;
		}

		void copy_data(const array<d, n>& o)
		{
			for (size_t i = 0; i < n; ++i)
				_array[i].copy_data(o._array[i]);
		}

		// TODO macros to generate subnode/dump_into ?

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
