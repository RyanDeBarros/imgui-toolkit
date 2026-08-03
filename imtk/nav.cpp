#include "nav.hpp"

#include <imgui.h>

namespace imtk::nav
{
	KeyboardButtonQuery shift()
	{
		return {
			.down = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift),
			.pressed = ImGui::IsKeyPressed(ImGuiKey_LeftShift) || ImGui::IsKeyPressed(ImGuiKey_RightShift)
		};
	}
	
	KeyboardButtonQuery ctrl()
	{
		return {
			.down = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl),
			.pressed = ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl)
		};
	}
	
	KeyboardButtonQuery alt()
	{
		return {
			.down = ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt),
			.pressed = ImGui::IsKeyPressed(ImGuiKey_LeftAlt) || ImGui::IsKeyPressed(ImGuiKey_RightAlt)
		};
	}

	MouseButtonQuery lmb()
	{
		return {
			.down = ImGui::IsMouseDown(ImGuiMouseButton_Left),
			.clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left),
			.double_clicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left),
			.dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left)
		};
	}

	MouseButtonQuery mmb()
	{
		return {
			.down = ImGui::IsMouseDown(ImGuiMouseButton_Middle),
			.clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Middle),
			.double_clicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Middle),
			.dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Middle)
		};
	}
	
	MouseButtonQuery rmb()
	{
		return {
			.down = ImGui::IsMouseDown(ImGuiMouseButton_Right),
			.clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Right),
			.double_clicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right),
			.dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Right)
		};
	}

	KeyboardButtonQuery escape()
	{
		return {
			.down = ImGui::IsKeyDown(ImGuiKey_Escape),
			.pressed = ImGui::IsKeyPressed(ImGuiKey_Escape)
		};
	}

	KeyboardButtonQuery enter()
	{
		return {
			.down = ImGui::IsKeyDown(ImGuiKey_Enter),
			.pressed = ImGui::IsKeyPressed(ImGuiKey_Enter)
		};
	}
}
