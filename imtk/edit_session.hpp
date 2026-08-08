#pragma once

#include "imtk/item_state.hpp"
#include "imtk/tick.hpp"

namespace imtk
{
	template<typename ty>
	class edit_session : public tick_processor
	{
		ty& _truth;
		ty _buffer = ty();
		ty _original = ty();

		bool _editing = false;
		bool _published = false;

	public:
		edit_session(ty& truth)
			: _truth(truth)
		{
		}

		ty& buffer()
		{
			return _buffer;
		}

		void pre_edit()
		{
			if (!_editing)
			{
				_buffer = _truth;
				_original = _buffer;
			}

			processed_this_frame();
			_published = false;
		}

		void post_edit(item_state state)
		{
			if (state.deactivated_after_edit())
			{
				_editing = false;
				_truth = _buffer;
				_published = true;
			}

			if (state.activated())
				_editing = true;
		}

	protected:
		void on_last_process_frame_plus_one() override
		{
			if (_editing)
			{
				_editing = false;
				_truth = _buffer;
				_published = true;
			}
		}

	public:
		bool modified() const
		{
			return _published && _buffer != _original;
		}

		bool consume_modified()
		{
			const bool m = modified();
			_published = false;
			return m;
		}

		void publish_reset(ty to)
		{
			_truth = to;
			_buffer = std::move(to);
			_published = true;
		}

		void cancel_editing()
		{
			_buffer = _truth;
			_original = _buffer;
			_editing = false;
			_published = false;
		}
	};
}
