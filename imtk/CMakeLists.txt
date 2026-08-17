target_sources(imgui-toolkit PRIVATE
	controls.cpp
	collapsing_section.cpp
	dock.cpp
	drag_drop.cpp
	errors.cpp
	id_scope.cpp
	item_result.cpp
	label_registry.cpp
	nav.cpp
	os_window.cpp
	outline.cpp
	overlays.cpp
	popup.cpp
	resource_loader.cpp
	simple_scopes.cpp
	texture.cpp
	tick.cpp
	color.cpp
	wrapper.cpp
)

add_subdirectory(desc)
add_subdirectory(prop)
add_subdirectory(w)
