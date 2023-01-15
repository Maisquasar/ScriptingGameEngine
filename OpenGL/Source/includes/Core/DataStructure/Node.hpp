#pragma once
#include <ImGui/imgui.h>
#include <string>
#include "../Maths/Maths.hpp"
#include "../Source/includes/LowRenderer/Transform.hpp"
#include "../Source/includes/LowRenderer/Mesh.hpp"
#include "../../Resources/SceneLoader.hpp"
#include "../../Resources/Components/RotationController.hpp"
#include "../../LowRenderer/Light.hpp"
#include "../../Resources/Component.hpp"
#include "../../Physic/Colliders.hpp"
#include "../../Resources/Components/CameraLock.hpp"
#include "../../Physic/Rigidbody.hpp"
#include "../../Resources/Components/PlayerScript.hpp"
#include "../../Resources/Components/MainMenu.hpp"
#include "../Source/includes/Resources/Components/PauseMenu.hpp"
class Node : public Selectable 
{
public:
	std::string Name; // The Name of the Node (Show on the Inspector).
	std::vector<Node*> children; // The list of Childrens of the Node.
	Node* parent = nullptr;	// The Parent of the Node, if null no parent.
	std::vector<Component*> Components;	// The Components of the Node.

	//Space information
	Transform transform; // The position/rotatin/scale of the Node.
	Node();
	virtual ~Node() {
		for (auto comp : Components)
		{
			delete comp;
		}
	}
	// Function to Delete the Node.
	void Delete();

	// Add a child with a Node in arg.
	void addChild(Node* node);
	// Add a Component.
	void AddComponent(Component*);

	// Set The World Transform.
	void SetWorldPosition(Vec3D pos);
	void SetWorldRotation(Vec3D rot);
	void SetWorldScale(Vec3D sca);

	//Update transform if it was changed
	void updateSelfAndChild();

	//Force update of transform even if local space don't change
	void forceUpdateSelfAndChild();

	// Update All Components of the Node.
	void UpdateComponents(Mat4D vp);

	// Function to Check if Node already had a component (Only this Node).
	template <typename T> bool AlreadyHad()
	{
		for (auto&& type : Components)
		{
			if (auto has = dynamic_cast<T*>(type))
				return true;
		}
		return false;
	}

	// Function to Check Number of Component of the Type of Component (Only this Node).
	template <typename T> bool AlreadyHad(int& number)
	{
		number = 0;
		for (auto&& type : Components)
		{
			if (auto has = dynamic_cast<T*>(type))
				number++;
		}
		if (number > 0)
			return true;
		return false;
	}

	// Function to Check if Node and childs already had a component.
	template <typename T> bool IsComponentInside()
	{
		bool a = false;
		if (AlreadyHad<T>())
		{
			return true;
		}
		for (auto child : children)
		{
			a = child->IsComponentInside<T>();
			if (a)
				return true;
		}
		return a;
		// Function to Check if Node already had a component.
	}

	// Function to Check if Node and childs already had a component and get Number.
	template <typename T> bool IsComponentInside(int& number)
	{
		bool a = false;
		if (AlreadyHad<T>())
		{
			number++;
			a = true;
		}
		for (auto child : children)
		{
			child->IsComponentInside<T>(number);
		}
		return a;
	}

	// Replace the Component. (for global change).
	template <typename T> void ReplaceComponent(T Comp, std::vector<bool> change)
	{
		for (auto c : Components) {
			if (auto type = dynamic_cast<T*>(c))
			{
				*type = Comp;
			}
		}
		for (auto child : children)
		{
			ReplaceComponent(Comp, change);
		}
	}
	template <> void ReplaceComponent<Mesh>(Mesh Comp, std::vector<bool> change)
	{
		for (auto c : Components) {
			if (auto type = dynamic_cast<Mesh*>(c))
			{
				if (change[0]) {
					auto temp = type->CurrentMaterial.NormalMap;
					type->CurrentMaterial = Comp.CurrentMaterial;
					type->CurrentMaterial.NormalMap = temp;
				}
				if (change[1])
					type->CurrentTexture = Comp.CurrentTexture;
				if (change[2] && Comp.CurrentShader != nullptr) {
					type->CurrentShader = Comp.CurrentShader;
					type->CurrentMaterial.NormalMap = Comp.CurrentMaterial.NormalMap;
				}
			}
		}
		for (auto child : children)
		{
			child->ReplaceComponent(Comp, change);
		}
	}
	//TODO: This with other components

	// Return the list of the component of the type given.
	template <typename T> std::list<T> GetComponentsInChildrens()
	{
		std::list<T> Types;
		for (auto comp : this->Components)
		{
			if (auto type = dynamic_cast<T>(comp))
				Types.push_back(type);
		}
		for (auto child : children)
		{
			std::list<T> tmp = child->GetComponentsInChildrens<T>();
			for (auto all : tmp)
			{
				Types.push_back(all);
			}
		}
		return Types;
	}

	// Return all Children on a list.
	std::list<Node*> GetAllChildrens();

	// Return the component (null if no inside)
	template <typename T> T* GetComponent()
	{
		for (auto comp : Components)
		{
			if (auto tmp = dynamic_cast<T*>(comp))
				return tmp;
		}
		return nullptr;
	}

	// Show Hirearchy.
	void ShowImGui(Node*& last, int index);

	// Save The Nide into file.
	void Save(std::ofstream& _file, std::string space);

	// Load Functions.
	static Node* Load(const char* data, uint32_t& pos, std::unordered_map<std::string, IResource*> res);
	void LoadComponent(const char* data, uint32_t& pos, std::unordered_map<std::string, IResource*> res); // LOAD
	static Node* Loop(const char* data, uint32_t size, std::unordered_map<std::string, IResource*> res);
	static Node* LoadScene(std::string filename, std::unordered_map<std::string, IResource*> res);

	bool IsHovered();
	void SetLastState(bool);

	void SetPause(bool* pause) { _isInpause = pause; }
	void SetGameState(GameState* value) { _gameState = value; }

	bool IsInPause() { return *_isInpause; }
	bool* GetPausePtr() { return _isInpause; }
	GameState GetGameState() { return *_gameState; }
	GameState* GetGameStatePointer() { return _gameState; }
	Node& operator=(const Node&) { return *this; }
private:
	bool _hover = false;
	bool _rightClicked = false;
	bool _lastState = false; // For ImGui Inpector
	bool _leftClicked = false;

	GameState* _gameState;
	bool* _isInpause = nullptr;

};



