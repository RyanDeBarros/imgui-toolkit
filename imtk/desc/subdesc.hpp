#pragma once

#include "imtk/datapath.hpp"
#include "imtk/key.hpp"

#include <imp/type_erasure.hpp>

namespace imtk::desc
{
	template<typename desc>
	struct sub
	{
		desc d;
		key key_;

		sub(key key, datapath_link link = {}) : key_(key), d(std::move(link)) {}

		const desc& operator*() const
		{
			return d;
		}

		desc& operator*()
		{
			return d;
		}

		const desc* operator->() const
		{
			return &d;
		}

		desc* operator->()
		{
			return &d;
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return d.resolve(path, type);
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			return d.describe(os, path);
		}

		bool query_dirty(const sub<desc>& disk) const
		{
			return d.query_dirty(disk.d);
		}

		void copy_data(const sub<desc>& o)
		{
			d.copy_data(o.d);
		}

		toml_node subnode(toml_node node) const
		{
			return node[encode_key(key_)];
		}

		template<typename value>
		void dump_into(toml::table& table, value&& value_) const
		{
			// TODO IMTK_DUMP_FIELDS should work on descs too: add dump()/load() to desc methods.
			table.insert_or_assign(encode_key(key_), std::forward<value>(value_));
		}
	};
}
