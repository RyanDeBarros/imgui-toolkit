#pragma once

#include "imtk/datapath.hpp"
#include "imtk/desc/set_action.hpp"

#include <imp/box.hpp>
#include <imp/undo_history.hpp>

namespace imtk::desc
{
	struct idoubler : public data_accessor
	{
		virtual ~idoubler() = default;
		virtual bool query_dirty() = 0;
		virtual imp::box copy_scratch() const = 0;
		virtual std::unique_ptr<imp::undo_action> scratch_undo_action(imp::box original) const = 0;
		virtual bool scratch_undo_action_query(imp::box original, std::unique_ptr<imp::undo_action>& action) const = 0;
	};

	template<typename d>
	struct doubler : public idoubler
	{
		d scratch;
		d disk;

		doubler() = default;
		doubler(d scratch, d disk) : scratch(std::move(scratch)), disk(std::move(disk)) {}

		void* resolve(datapath_view path, imp::type_erasure type) override
		{
			return scratch.resolve(path, type);
		}

		void describe(std::ostream& os, datapath_view path) const override
		{
			scratch.describe(os, path);
		}

		bool query_dirty() override
		{
			return scratch.query_dirty(disk);
		}

		imp::box copy_scratch() const
		{
			return imp::make_box<d>(desc::clone_data(scratch));
		}

		std::unique_ptr<imp::undo_action> scratch_undo_action(imp::box original) const override
		{
			if (auto og = original.consume<d>())
				return std::make_unique<desc::set_action<d, void>>(datapath_view(), std::move(*og), desc::clone_data(scratch));
			else
				return nullptr;
		}

		bool scratch_undo_action_query(imp::box original, std::unique_ptr<imp::undo_action>& action) const override
		{
			if (auto og = original.as<d>())
			{
				if (scratch.query_dirty(*og))
				{
					if (auto og = original.consume<d>())
						action = std::make_unique<desc::set_action<d, void>>(datapath_view(), std::move(*og), desc::clone_data(scratch));
					else
						action = nullptr;

					return true;
				}
			}

			return false;
		}

		void write_to_disk()
		{
			disk.copy_data(scratch);
		}

		void load_from_disk()
		{
			scratch.copy_data(disk);
		}

		void reset_scratch()
		{
			scratch = d();
		}
	};
}
