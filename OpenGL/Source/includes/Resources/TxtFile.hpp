#pragma once
#include "IResource.hpp"
#include "SceneLoader.hpp"

class TxtFile : public IResource
{
public:
	TxtFile(const char* file);
	~TxtFile() {}

	const char* Content;

	void ShowResource() override
	{
		float wrap_width = ImGui::GetWindowWidth();
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
		ImGui::TextUnformatted(Content);
		ImGui::PopTextWrapPos();
	}

private:

};
