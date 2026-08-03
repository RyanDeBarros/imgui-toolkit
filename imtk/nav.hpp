#pragma once

namespace imtk::nav
{
	struct KeyboardButtonQuery
	{
		bool down;
		bool pressed;
	};

	struct MouseButtonQuery
	{
		bool down;
		bool clicked;
		bool double_clicked;
		bool dragging;
	};

	extern KeyboardButtonQuery shift();
	extern KeyboardButtonQuery ctrl();
	extern KeyboardButtonQuery alt();

	extern MouseButtonQuery lmb();
	extern MouseButtonQuery mmb();
	extern MouseButtonQuery rmb();

	extern KeyboardButtonQuery escape();
	extern KeyboardButtonQuery enter();
}
