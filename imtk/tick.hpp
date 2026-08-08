#pragma once

namespace imtk
{
	using frame_number = unsigned long long;

	extern void begin_frame();
	extern void end_frame();
	extern frame_number frame();

	class tick_processor
	{
		frame_number last_frame_processed = {};

	public:
		tick_processor();
		tick_processor(const tick_processor&);
		tick_processor(tick_processor&&) noexcept;
		~tick_processor();

		virtual void on_last_process_frame() {}

	protected:
		void processed_this_frame();

	private:
		friend void end_frame();
		void check_for_last_processed_frame();
	};
}
