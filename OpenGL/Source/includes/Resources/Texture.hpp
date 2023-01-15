#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <ImGui/imgui.h>
#include "IResource.hpp"
#include "../Core/Debug/Log.hpp"
#include "../Core/Maths/Maths.hpp"
#include "../Resources/SceneLoader.hpp"


static const char* sTexFilter[] { "Linear", "Nearest" };
static const char* sTexWrapping[] { "Repeat", "Mirrored Repeat",  "Clamp to Edge", "Clamp to Border" };
static int sTexFilIdx = 0;
static int sTexWraIdx = 0;
class Texture : public IResource
{
public:
	GLuint TextureData = 0;
	int Index = 0;
	GLuint Sampler = 0;
	int Filter = GL_LINEAR;
	int Wrapping = GL_REPEAT;
	unsigned char* data;
	int Width, Height;
	bool SemiLoaded = false;

	std::string Name;

	Texture();
	Texture(int index);
	~Texture();

	// Load Texture from file.
	void LoadTexture(std::string);
	void Initialize();
	// Bind Sampler of the texture.
	void Bind();
	// Setters.
	void SetFilter(int Filter);
	void SetWrapping(int Wrapping);

	// Save/Load.
	void Save(std::string space, std::string& line);
	static Texture* Load(const char* data, uint32_t& pos);

	void ShowImGui();
	// Draw Imgui.
	void ShowResource() override
	{
		ImGui::PushID(TextureData);
		ImGui::Image((ImTextureID)(uint64_t)TextureData, ImVec2{ 64, 64 }, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 1));
		ImGui::SameLine();
		ImGui::BeginGroup();
		// Texture Filter.
		// Set The Preview Text For texture Filter.
		std::string PreviewTxt;
		if (this->Filter == GL_LINEAR) {
			PreviewTxt = sTexFilter[0];
			sTexFilIdx = 0;
		}
		else if (this->Filter == GL_NEAREST) {
			PreviewTxt = sTexFilter[1];
			sTexFilIdx = 1;
		}
		ImGui::PushItemWidth(128);
		if (ImGui::BeginCombo("Texture Filter", PreviewTxt.c_str()))
		{
			for (int n = 0; n < IM_ARRAYSIZE(sTexFilter); n++)
			{
				const bool is_selected = (sTexFilIdx == n);
				if (ImGui::Selectable(sTexFilter[n], is_selected)) {
					sTexFilIdx = n;
					if (sTexFilIdx == 0)
						this->SetFilter(GL_LINEAR);
					else if (sTexFilIdx == 1)
						this->SetFilter(GL_NEAREST);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::NewLine();
		// Texture Wrapping.
		// Set The Preview Text For texture Wrapping.
		if (this->Wrapping == GL_REPEAT) {
			PreviewTxt = sTexWrapping[0];
			sTexWraIdx = 0;
		}
		else if (this->Wrapping == GL_MIRRORED_REPEAT) {
			PreviewTxt = sTexWrapping[1];
			sTexWraIdx = 1;
		}
		else if (this->Wrapping == GL_CLAMP_TO_EDGE)
		{
			PreviewTxt = sTexWrapping[2];
			sTexWraIdx = 2;
		}
		else if (this->Wrapping == GL_CLAMP_TO_BORDER)
		{
			PreviewTxt = sTexWrapping[3];
			sTexWraIdx = 3;
		}
		if (ImGui::BeginCombo("Texture Wrapping", PreviewTxt.c_str()))
		{
			for (int n = 0; n < IM_ARRAYSIZE(sTexWrapping); n++)
			{
				const bool is_selected = (sTexWraIdx == n);
				if (ImGui::Selectable(sTexWrapping[n], is_selected)) {
					sTexWraIdx = n;
					if (sTexWraIdx == 0)
						this->SetWrapping(GL_REPEAT);
					else if (sTexWraIdx == 1)
						this->SetWrapping(GL_MIRRORED_REPEAT);
					else if (sTexWraIdx == 2)
						this->SetWrapping(GL_CLAMP_TO_EDGE);
					else if (sTexWraIdx == 3)
						this->SetWrapping(GL_CLAMP_TO_BORDER);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::EndGroup();
		ImGui::Text("Size : %d x %d", this->Width, this->Height);
	}
private:
};
