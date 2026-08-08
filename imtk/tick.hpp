#pragma once

namespace imtk
{
	using frame_number = unsigned long long;

	extern void begin_frame();
	extern void end_frame();
	extern frame_number frame();

	enum class tick_process_phase
	{
		submit_edit,
		check_undo,
		query_dirty
	};

	class tick_processor
	{
		frame_number _last_frame_processed = {};
		tick_process_phase _phase;

	public:
		tick_processor(tick_process_phase phase);
		tick_processor(const tick_processor&);
		tick_processor(tick_processor&&) noexcept;
		~tick_processor();
		tick_processor& operator=(const tick_processor&);
		tick_processor& operator=(tick_processor&&) noexcept;

		virtual void on_last_process_frame() {}

	protected:
		void processed_this_frame();

	private:
		friend void end_frame();
		void check_for_last_processed_frame();

	public:
		void set_phase(tick_process_phase phase);
	};
}
