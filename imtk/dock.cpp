#include "dock.hpp"

#include <imgui_internal.h>

namespace imtk
{
    void dock_node::split_layout(ImGuiID id) const
    {
        if (!_names.empty())
        {
            for (const auto& name : _names)
                ImGui::DockBuilderDockWindow(name.c_str(), id);
        }
        else if (_first && _second)
        {
            ImGuiID dock_first, dock_second;
            dock_second = ImGui::DockBuilderSplitNode(id, _direction, 1.f - _split_factor, nullptr, &dock_first);
            _first->split_layout(dock_first);
            _second->split_layout(dock_second);
        }
    }

    void dock_node::setup_layout(ImGuiID dockspace_id) const
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->WorkSize);
        split_layout(dockspace_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    std::unique_ptr<dock_node> dock::make_branch(ImGuiDir direction, std::unique_ptr<dock_node>&& first, std::unique_ptr<dock_node>&& second, float split_factor)
    {
        dock_node node;
        node._direction = direction;
        node._first = std::move(first);
        node._second = std::move(second);
        node._split_factor = split_factor;
        return std::make_unique<dock_node>(std::move(node));
    }

    std::unique_ptr<dock_node> dock::make_leaf(std::vector<std::string>&& names)
    {
        dock_node node;
        node._names = std::move(names);
        return std::make_unique<dock_node>(std::move(node));
    }
}
