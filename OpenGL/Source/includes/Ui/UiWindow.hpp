#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "../Core/Debug/Log.hpp"

class UiWindow
{
public:
	UiWindow() {};
	~UiWindow() {};

	virtual void Show() {};
	ImGuiWindow* GetWindow() { return _window; }
protected:
	ImGuiWindow* _window = nullptr;

};