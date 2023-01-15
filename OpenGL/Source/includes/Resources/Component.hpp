#pragma once
class Node;
class PythonScript;
#include <ImGui/imgui.h>
#include <ostream>
#include <unordered_map>
#include "IResource.hpp"
#include "../LowRenderer/Transform.hpp"
#include "SceneLoader.hpp"
#include "../Core/DataStructure/Serializer.hpp"
#include <pybind11/pybind11.h>

class Selectable
{
	virtual void Test() {}
};

class Component
{
public:

	Component();
	~Component();

	PythonScript* Script;

	bool enable = true;
	bool windowOpen = true;
	bool multiple = true;
	std::string ComponentName;
	Node* gameObject = nullptr;

	virtual void ShowComponent() { ImGui::Text("..."); }
	virtual void Start() {}
	// Use for Editor Only
	virtual void EditorUpdate() {}
	// Use for Game Only
	virtual void GameUpdate() {}
	// Use for All Game Modes
	virtual void Update() {}

	virtual void Initialize() { }
	virtual void Save(std::string space, std::string& line) {}
	virtual Component* Load(const char* data, uint32_t& pos) { return nullptr; }


	bool IsInPause();
	bool* GetPausePtr();
	GameState GetGameState();
	Transform* GetTransform();
	std::unordered_map<std::string, IResource*>* GetResourcesManager();
protected:
	Serializer _serializer;
};

class ComponentsData
{
public:
	ComponentsData();
	~ComponentsData();
	void Initialize();
	void NewComponent(Component* comp) { Datas.push_back(comp); }

	std::vector<Component*> Datas;
private:

};
extern ComponentsData CData;