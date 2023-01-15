#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_internal.h>

#include <iostream>
#include <vector>
#include <format>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

#include "Debug/Assertions.hpp"
#include "Debug/Log.hpp"
#include "Maths/Maths.hpp"
#include "../Resources/ResourcesManager.hpp"
#include "../Resources/Texture.hpp"
#include "../Resources/Shader.hpp"
#include "../LowRenderer/Camera.hpp"
#include "../Core/Input.hpp"
#include "../Ui/Inspector.hpp"
#include "../LowRenderer/Transform.hpp"
#include "../Resources/SceneLoader.hpp"
#include "../LowRenderer/LightManager.hpp"
#include "../LowRenderer/FrameBuffer.hpp"
#include "../LowRenderer/ShadowMap.hpp"
#include "../Core/DataStructure/PlayerPrefs.hpp"
#include "../Ui/EditorUi.hpp"
#include "../Resources/PythonScriptManager.hpp"

#define MULTITHREAD 1
#define JSON 1

struct AppInitializer
{
	unsigned int width;
	unsigned int height;
	const char* name;
};

struct Settings
{
	bool GoToNode = false; // Setting to go to Node whe selected
	bool ShowCollision = false; // Boolean to Show/Hide Collsion
	bool Vsync = true;	// Vertical sync Boolean.
};

class App
{
private:
	ResourcesManager resourcesManager;
	CollisionManager collisionManager = CollisionManager(new GameState());
	LightManager lightManager;
	EditorUi editorUi;

	int _width = 800; // Width of the Window.
	int _height = 600; // Height of the Window.

	bool _close = false; // Boolean to Close Window.

	bool _isInPause = false;

public:
	Settings settings;
	PythonScriptManager ScriptManager;

	float TimeSinceStart = 0.f; // Time since Start.

	bool WindowChangeSize = true;

	std::thread::id MainThreadId = std::this_thread::get_id();

	GLFWwindow* Window = nullptr;
	GLuint ShaderProgram = 0;
	GameState State; // The State of the Game (Play/Pause)

	Camera camera;
	FrameBuffer frameBuffer;

	Node* SceneNode = new Node(); // The Main Node.

	PlayerPrefs PlayerPrefs;

	std::map<Node*, std::string> WaitingList; // List of mesh that awaits loading

	std::chrono::steady_clock::time_point LoadingTime;

	App();
	~App();

	// Init the App.
	void InitApp(AppInitializer data);
	void GlfwInit();
	void InitWindow(int Width, int Height, const char* Name);
	void GladLoad();
	ImVec2 GetWindowSize();

	void LoadResources();
	// Load All textures in the asset file. Multithreading ON
	void LoadAllTextures(); 
	// Load All Models in the asset file. Multithreading ON
	void LoadAllModels();
	// Load all Scripts.
	void LoadAllScripts();
	// Load and Init all Shaders.
	void InitShader(); 
	// Load the Scene by the SceneName.
	void InitScene(); 
	// Add into waiting list for meshs
	void AddToWaitlist(Node* node, std::string name) { WaitingList[node] = name; }

	// Callback on Mouse Scroll.
	static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset); 

	// Main Update.
	void Update();
	void ClearApp();
	void NewFrame(bool mouseCaptured);
	void BeginPlay();
	void EndPlay();

	// Getters
	ResourcesManager* GetResourcesManager() { return &resourcesManager; }
	CollisionManager* GetColliderManager() { return &collisionManager; }
	LightManager* GetLightManager() { return &lightManager; }
	Inspector* GetInspector() { return &editorUi.inspector; }
	bool* GetPause() { return &_isInPause; }

	// Scene Loading/Saving
	void SaveScene(std::string);
	void LoadScene(std::string);
	void CopyToClipboard(Node* node);
	void LoadMultithread();

	void CloseApp() { _close = true; }
};

extern App app;
