#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Source/includes/LowRenderer/Light.hpp"
#include "../Core/DataStructure/Node.hpp"

#define LIGHT_SIZE 8

class LightManager
{
public:
	std::list<DirectionalLight*> DirLights;
	std::list<PointLight*> PointLights;
	std::list<SpotLight*> SpotLights;

	LightManager();
	~LightManager();

	// Add Light Component inside a Node.
	template <typename T> bool AddLightComponent(Node* node);
	template <>bool AddLightComponent<DirectionalLight>(Node* node);
	template <>bool AddLightComponent<PointLight>(Node* node);
	template <>bool AddLightComponent<SpotLight>(Node* node);

	// return true if Component can Be add.
	// Check for size of all lights.
	template <typename T> bool CanAddLight(Node* node);
	template <>	bool CanAddLight<DirectionalLight>(Node* node);
	template <>	bool CanAddLight<PointLight>(Node* node);
	template <>	bool CanAddLight<SpotLight>(Node* node);

	// return a free index.
	template <typename T> int GetFreeIndex();
	template <>	int GetFreeIndex<DirectionalLight>();
	template <>	int GetFreeIndex<PointLight>();
	template <>	int GetFreeIndex<SpotLight>();

	template <typename T>bool CanBeAdd(Node* node);

	bool Add(Node* node);

	void Update(Shader* ShaderProgram);

	// return a copy of the node.
	Node* CopyNode(Node* from);

	// return a copy of the node components.
	Node* CopyComponents(Node* from);

private:

};
