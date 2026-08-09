#pragma once

#include "imtk/desc/datapath.hpp"

namespace imtk::desc
{
	namespace internal
	{
		template<typename ty>
		void describe(std::ostream& os, datapath_view path, const char* name, const ty& field)
		{
			if constexpr (requires(ty t, std::ostream os, datapath_view path) { t.describe(os, path); })
			{
				os << name << ".";
				if (path.empty())
					os << "<error>";
				else
					field.describe(os, path);
			}
			else
				os << name;
		}
	}

#define _IMTK_SUBPATH_ENUM_ENTRY(field) _E_##field,
#define _IMTK_SUBPATH_STRUCT_ENTRY(field) static constexpr imtk::datapath::step field = imtk::datapath::step(_E_##field);
#define _IMTK_SUBPATH_PATH_GET(field) case _E_##field: return field.resolve(path.next(), type);
#define _IMTK_SUBPATH_PRINT_PATH(field) case _E_##field: imtk::desc::internal::describe(os, path.next(), #field, field); break;
#define _IMTK_SUBPATH_QUERY_DIRTY(field) if (field.query_dirty(disk.field)) return true;
#define _IMTK_SUBPATH_COPY_DATA(field) field.copy_data(o.field);
	// TODO make note that IMTK_DESCRIPTOR_BODY() must be the first thing in the descriptor class definition, or at least before the generator members are declared
#define IMTK_DESCRIPTOR_BODY(Klass, GENERATOR) \
		public: imtk::datapath_link link; \
		private: enum : int { GENERATOR(_IMTK_SUBPATH_ENUM_ENTRY) }; \
		public: struct { GENERATOR(_IMTK_SUBPATH_STRUCT_ENTRY) } subpaths; \
		void* resolve(imtk::datapath_view path, std::type_index type) \
		{ \
			if (path.empty()) \
				return typeid(decltype(*this)) == type ? static_cast<void*>(this) : nullptr; \
			switch (path.step()) \
			{ \
				GENERATOR(_IMTK_SUBPATH_PATH_GET); \
			default: \
				return nullptr; \
			} \
		} \
		void describe(std::ostream& os, imtk::datapath_view path) const \
		{ \
			if (path.empty()) \
				os << "<error>"; \
			else \
			{ \
				switch (path.step()) \
				{ \
					GENERATOR(_IMTK_SUBPATH_PRINT_PATH); \
				default: \
					os << "<error>"; \
				} \
			} \
		} \
		bool query_dirty(const Klass& disk) const { GENERATOR(_IMTK_SUBPATH_QUERY_DIRTY); return false; } \
		void copy_data(const Klass& o) { GENERATOR(_IMTK_SUBPATH_COPY_DATA); }

#define _IMTK_DRAW_FIELD(field) desc.field.draw();
#define IMTK_DRAW_FIELDS(GENERATOR) GENERATOR(_IMTK_DRAW_FIELD);

#define _IMTK_LOAD_FIELD(field) desc.field.load(node);
#define IMTK_LOAD_FIELDS(GENERATOR) GENERATOR(_IMTK_LOAD_FIELD)

#define _IMTK_DUMP_FIELD(field) desc.field.dump(table);
#define IMTK_DUMP_FIELDS(GENERATOR) GENERATOR(_IMTK_DUMP_FIELD)
}
