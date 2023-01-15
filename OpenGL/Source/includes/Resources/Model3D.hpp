#pragma once
#include <ostream>
#include <iostream>
#include <string>
#include "../Resources/SceneLoader.hpp"
#include "../LowRenderer/Mesh.hpp"
#include "../Core/Maths/Maths.hpp"
#include "../Core/Debug/Log.hpp"
#include "IResource.hpp"
#include "../Core/DataStructure/Node.hpp"
#include "../Resources/SceneLoader.hpp"
class ResourcesManager;
#include "ResourcesManager.hpp"
#define MeshPath "Assets/meshs/"
#define TexPath "Assets/textures/"

using namespace Debug;
class Model3D : public IResource, public Node
{
public:
	std::map<std::string, Material> Materials;

	Model3D();
	~Model3D();

	// Model Loading.
	void LoadModel(std::string, ResourcesManager&);

	// Read the Obj file.
	void ModelLoop(const char* data, const int32_t& size, ResourcesManager&);

	// Read the Mtl file.
	void MaterialLoop(const char* data, const int32_t& size, ResourcesManager&);

	bool IsLoaded() override;

};