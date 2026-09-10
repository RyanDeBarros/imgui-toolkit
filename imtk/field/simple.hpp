#pragma once

#include "imtk/key.hpp"
#include "imtk/serializer.hpp"

namespace imtk::field
{
	template<typename ty>
	struct simple
	{
		ty value;
		imtk::key key;

		simple(ty value, imtk::key key)
			: value(std::move(value)), key(key)
		{
		}

		void load(imtk::toml_node node)
		{
			imtk::serializer<ty>{}.load(value, node[imtk::encode_key(key)]);
		}

		void dump(toml::table& table)
		{
			table.insert_or_assign(imtk::encode_key(key), imtk::serializer<ty>{}.dump(value));
		}

		const ty& operator*() const
		{
			return value;
		}

		ty& operator*()
		{
			return value;
		}

		const ty* operator->() const
		{
			return &value;
		}

		ty* operator->()
		{
			return &value;
		}

		const ty* value_ptr() const
		{
			return &value;
		}

		ty* value_ptr()
		{
			return &value;
		}
	};

}

#define _IMTK_LOAD_SIMPLE_FIELD(F) F.load(node);
#define IMTK_LOAD_SIMPLE_FIELDS(GENERATOR) GENERATOR(_IMTK_LOAD_SIMPLE_FIELD)

#define _IMTK_DUMP_SIMPLE_FIELD(F) F.dump(table);
#define IMTK_DUMP_SIMPLE_FIELDS(GENERATOR) GENERATOR(_IMTK_DUMP_SIMPLE_FIELD)

#define _IMTK_LOAD_SIMPLE_FIELDS_IMPL(GENERATOR) void load(imtk::toml_node node) { IMTK_LOAD_SIMPLE_FIELDS(GENERATOR) }
#define _IMTK_DUMP_SIMPLE_FIELDS_IMPL(GENERATOR) void dump(toml::table& table) { IMTK_DUMP_SIMPLE_FIELDS(GENERATOR) }
#define IMTK_LOAD_DUMP_SIMPLE_FIELDS_IMPL(GENERATOR) _IMTK_LOAD_SIMPLE_FIELDS_IMPL(GENERATOR) _IMTK_DUMP_SIMPLE_FIELDS_IMPL(GENERATOR)
