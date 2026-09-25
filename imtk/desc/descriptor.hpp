#pragma once

#include "imtk/datapath.hpp"

#include <imp/macros.hpp>
#include <imp/type_erasure.hpp>

namespace imtk::desc
{
    namespace internal
    {
        template<typename ty>
        void describe(std::ostream& os, datapath_view path, const std::string_view name, const ty& field)
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

    template<typename d>
    d clone_data(const d& desc)
    {
        d copy{};
        copy.copy_data(desc);
        return copy;
    }

    // TODO documentation for how to use fields/descriptors, and how to use generators/macros
}

#define _IMTK_FIELD_DECL(type, field) IMP_UNPAREN(type) field;
#define _IMTK_SUBPATH_ENUM_ENTRY(_, field) _E_##field,
#define _IMTK_SUBPATH_STRUCT_ENTRY(_, field) static constexpr imtk::datapath::step field = imtk::datapath::step(_E_##field);
#define _IMTK_SUBPATH_PATH_GET(_, field) case _E_##field: return field.resolve(path.next(), type);
#define _IMTK_SUBPATH_PRINT_PATH(_, field) case _E_##field: imtk::desc::internal::describe(os, path.next(), #field, field); break;
#define _IMTK_SUBPATH_QUERY_DIRTY(_, field) if (field.query_dirty(disk.field)) return true;
#define _IMTK_SUBPATH_COPY_DATA(_, field) field.copy_data(o.field);
#define IMTK_DESCRIPTOR_BODY(Klass, GENERATOR) \
		public: imtk::datapath_link link; \
		    GENERATOR(_IMTK_FIELD_DECL)\
		private: \
            enum : int { GENERATOR(_IMTK_SUBPATH_ENUM_ENTRY) }; \
		public: \
            struct { GENERATOR(_IMTK_SUBPATH_STRUCT_ENTRY) } subpaths; \
		    void* resolve(imtk::datapath_view path, imp::type_erasure type) \
		    { \
			    if (path.empty()) \
				    return imp::resolve_type(type, this); \
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

#define _IMTK_DRAW_FIELD(_, field) desc.field.draw();
#define IMTK_DRAW_FIELDS(GENERATOR) GENERATOR(_IMTK_DRAW_FIELD);

#define _IMTK_LOAD_FIELD(_, field) desc.field.load(node);
#define IMTK_LOAD_FIELDS(GENERATOR) GENERATOR(_IMTK_LOAD_FIELD)

#define _IMTK_DUMP_FIELD(_, field) desc.field.dump(table);
#define IMTK_DUMP_FIELDS(GENERATOR) GENERATOR(_IMTK_DUMP_FIELD)

#define _IMTK_DESC_TUPLE_MAKE_RESETTER(field) resetters.push_back(imtk::prop::make_resettable_value(field));
#define _IMTK_DESC_TUPLE_CONSUME_PUBLISHED_FROM(field) \
        if (auto _og = field.edit.consume_published_from()) \
        { \
            _consumed = true; \
            _original.value = _og.value_or(field.value); \
        }
#define IMTK_DESC_TUPLE_METHODS(Klass, GENERATOR) \
        public: \
            auto make_resetter() \
            { \
                std::vector<std::unique_ptr<imtk::prop::iresettable>> resetters; \
                GENERATOR(_IMTK_DESC_TUPLE_MAKE_RESETTER) \
                return std::make_unique<imtk::prop::resettable_row>(std::move(resetters)); \
            } \
            void check_undo_action() \
            { \
                Klass _original; \
                bool _consumed = false; \
                GENERATOR(_IMTK_DESC_TUPLE_CONSUME_PUBLISHED_FROM); \
                if (_consumed) \
                    imtk::desc::push_set_action(link.compute_path(), std::move(_original), imtk::desc::clone_data(*this)); \
            }
