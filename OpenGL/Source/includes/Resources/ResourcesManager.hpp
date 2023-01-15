#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <typeinfo>
#include <ImGui/imgui.h>
#include <unordered_map>
class Texture;
#include "Texture.hpp"
class IResource;
#include "IResource.hpp"
class Shader;
#include "Shader.hpp"
class Model3D;
#include "Model3D.hpp"
#include "TxtFile.hpp"
#include "../Source/includes/Core/DataStructure/ThreadManager.hpp"

using namespace Debug;

class ResourcesManager
{
private:

public:
	ThreadManager threadManager;
	std::unordered_map<std::string, IResource*> Resources;
	int TexId = 0;

	ResourcesManager();
	~ResourcesManager();
	// Creat/Get
	template<typename T> T Create(std::string);
	template<typename T> T Get(std::string filename);

	void CreateMesh(Mesh mesh, std::string filename);
	void MultithreadModels(std::string filename);
	void MultithreadTexture(std::string filename);

	void Delete(std::string filename);
	// Getters
	void GetAllModels(std::vector<Model3D>& models);
	void GetAllMeshs(std::vector<Mesh*>& models);

	// ImGui Draw
	Texture* TexturePopup()
	{
		if (ImGui::BeginPopupModal("Textures"))
		{
			for (auto&& res : Resources)
			{
				if (auto tex = dynamic_cast<Texture*>(res.second))
				{
					if (ImGui::ImageButton((ImTextureID)(uint64_t)tex->TextureData, ImVec2(32, 32))) {
						ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
						return tex;
					}
					ImGui::SameLine();
				}
			}
			ImGui::EndPopup();
		}
		return nullptr;
	}
};
