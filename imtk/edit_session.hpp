#pragma once

#include "imtk/item_state.hpp"

namespace imtk
{
	template<typename ty>
	class edit_session
	{
		ty& _truth;
		ty _buffer = ty();
		ty _original = ty();

		bool _editing = false;
		bool _seen_this_frame = false;
		bool _published = false;

	public:
		edit_session(ty& truth)
			: _truth(truth)
		{
		}

		void pre_edit()
		{
			if (!_editing)
			{
				_buffer = _truth;
				_original = _buffer;
			}

			_seen_this_frame = true;
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

		void draw_finalize()
		{
			if (!_seen_this_frame && _editing)
			{
				_editing = false;
				_truth = _buffer;
				_published = true;
			}

			_seen_this_frame = false;
		}

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
