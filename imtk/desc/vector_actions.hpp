#pragma once

#include "imtk/datapath.hpp"
#include "imtk/logger.hpp"
#include "imtk/printer.hpp"

#include "imtk/desc/containers.hpp"

#include <imp/undo_history.hpp>
#include <imp/fixed_array.hpp>

#include <sstream>

namespace imtk::desc
{
	template<typename element_ty, typename printer = standard_printer<element_ty>>
	struct vector_delete_action : public imp::undo_action
	{
		using list_ty = vector<element_ty>;

		datapath list_path;
		size_t delete_index;
		element_ty deleted_element;

		vector_delete_action(datapath_view list_path, size_t delete_index, element_ty deleted_element)
			: list_path(list_path), delete_index(delete_index), deleted_element(std::move(deleted_element))
		{
		}

		bool forward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (delete_index < ref_vector.size())
				{
					deleted_element = std::move(ref_vector[delete_index]);
					ref_vector.remove(delete_index);
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Redo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", delete@index=" << delete_index;
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", delete@element=";
				printer{}(ss, deleted_element);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		bool backward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (delete_index <= ref_vector.size())
				{
					ref_vector.insert(delete_index, clone_data(deleted_element));
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Undo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", re-insert@index=" << delete_index;
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", re-insert@element=";
				printer{}(ss, deleted_element);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		size_t empirical_size() const override
		{
			return sizeof(*this);
		}
	};

	template<typename element_ty, typename printer = standard_printer<element_ty>>
	void execute_vector_delete_action(datapath_view list_path, size_t delete_index)
	{
		imp::undo_history::active_instance().execute(std::make_unique<vector_delete_action<element_ty, printer>>(list_path, delete_index, element_ty{}));
	}

	template<typename element_ty, typename printer = standard_printer<element_ty>>
	struct vector_insert_action : public imp::undo_action
	{
		using list_ty = vector<element_ty>;

		datapath list_path;
		size_t insert_index;
		element_ty inserted_element;

		vector_insert_action(datapath_view list_path, size_t insert_index, element_ty inserted_element)
			: list_path(list_path), insert_index(insert_index), inserted_element(std::move(inserted_element))
		{
		}

		bool forward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (insert_index <= ref_vector.size())
				{
					ref_vector.insert(insert_index, clone_data(inserted_element));
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Redo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", insert@index=" << insert_index;
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", insert@element=";
				printer{}(ss, inserted_element);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		bool backward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (insert_index < ref_vector.size())
				{
					inserted_element = std::move(ref_vector[insert_index]);
					ref_vector.remove(insert_index);
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Undo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", re-delete@index=" << insert_index;
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", re-delete@element=";
				printer{}(ss, inserted_element);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		size_t empirical_size() const override
		{
			return sizeof(*this);
		}
	};

	template<typename element_ty, typename printer = standard_printer<element_ty>>
	void execute_vector_insert_action(datapath_view list_path, size_t insert_index, element_ty obj = {})
	{
		imp::undo_history::active_instance().execute(std::make_unique<vector_insert_action<element_ty, printer>>(list_path, insert_index, std::move(obj)));
	}

	template<typename element_ty>
	struct vector_move_action : public imp::undo_action
	{
		using list_ty = vector<element_ty>;

		datapath list_path;
		size_t src_index;
		size_t dst_index;

		vector_move_action(datapath_view list_path, size_t src_index, size_t dst_index)
			: list_path(list_path), src_index(src_index), dst_index(dst_index)
		{
		}

		bool forward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (std::max(src_index, dst_index) < ref_vector.size())
				{
					auto moved = std::move(ref_vector[src_index]);
					ref_vector.remove(src_index);
					ref_vector.insert(dst_index, std::move(moved));
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Redo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", from_index=" << src_index << ", to_index=" << dst_index << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		bool backward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);
				if (std::max(src_index, dst_index) < ref_vector.size())
				{
					auto moved = std::move(ref_vector[dst_index]);
					ref_vector.remove(dst_index);
					ref_vector.insert(src_index, std::move(moved));
					success = true;
				}
			}

			std::stringstream ss;
			ss << "Undo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", from_index=" << dst_index << ", to_index=" << src_index << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		size_t empirical_size() const override
		{
			return sizeof(*this);
		}
	};

	template<typename element_ty>
	void execute_vector_move_action(datapath_view list_path, size_t src_index, size_t dst_index)
	{
		imp::undo_history::active_instance().execute(std::make_unique<vector_move_action<element_ty>>(list_path, src_index, dst_index));
	}

	template<typename element_ty>
	struct vector_resize_action : public imp::undo_action
	{
		using list_ty = vector<element_ty>;

		datapath list_path;
		size_t initial_size;
		size_t final_size;
		imp::fixed_array<element_ty> erased;

		vector_resize_action(datapath_view list_path, size_t initial_size, size_t final_size)
			: list_path(list_path), initial_size(initial_size), final_size(final_size), erased(std::max(initial_size, final_size) - std::min(initial_size, final_size))
		{
		}

		bool forward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);

				if (initial_size < final_size)
				{
					ref_vector.resize(final_size);
					for (size_t i = initial_size; i < final_size; ++i)
						ref_vector[i].copy_data(erased[i - initial_size]);
				}
				else if (initial_size > final_size)
				{
					ref_vector.resize(initial_size);
					for (size_t i = final_size; i < initial_size; ++i)
						erased[i - final_size].copy_data(ref_vector[i]);
					ref_vector.resize(final_size);
				}

				success = true;
			}

			std::stringstream ss;
			ss << "Redo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", from_size=" << initial_size << ", to_size=" << final_size << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		bool backward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(list_path, imp::erase_type<list_ty>()))
			{
				auto& ref_vector = *static_cast<list_ty*>(var);

				if (initial_size < final_size)
				{
					ref_vector.resize(final_size);
					for (size_t i = initial_size; i < final_size; ++i)
						erased[i - initial_size].copy_data(ref_vector[i]);
					ref_vector.resize(initial_size);
				}
				else if (initial_size > final_size)
				{
					ref_vector.resize(initial_size);
					for (size_t i = final_size; i < initial_size; ++i)
						ref_vector[i].copy_data(erased[i - final_size]);
				}

				success = true;
			}

			std::stringstream ss;
			ss << "Undo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(list_path) << ", from_size=" << final_size << ", to_size=" << initial_size << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		size_t empirical_size() const override
		{
			return sizeof(*this) + erased.length() * sizeof(element_ty);
		}
	};

	template<typename element_ty>
	void execute_vector_resize_action(datapath_view list_path, size_t initial_size, size_t final_size)
	{
		imp::undo_history::active_instance().execute(std::make_unique<vector_resize_action<element_ty>>(list_path, initial_size, final_size));
	}
}
