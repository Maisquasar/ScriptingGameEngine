#pragma once
#include "Maths/Maths.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <ImGui/imgui.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
class Input
{
public:
	GLFWwindow* Window;

	// Inputs
	bool MouseCaptured = false;
	double MouseX = 0.0;
	double MouseY = 0.0;
	float MouseDeltaX = 0.0;
	float MouseDeltaY = 0.0;
	float Scroll = 0.f;

	int ScaleMode = 0;
	float DeltaX = 0;
	float DeltaY = 0;
	bool Forward = 0;
	bool Backward = 0;
	bool Up = 0;
	bool Down = 0;
	bool Right = 0;
	bool Left = 0;
	bool Ortho = 0;
	bool Shift = 0;
	float MouseScroll = 0;

	bool Wireframe = false;
	bool FullScreen = false;


	int _wndPos[2];
	int _wndSize[2];
	GLFWmonitor* _monitor = nullptr;

	Input();
	Input(GLFWwindow* window, GameState* state);
	~Input();

	void CameraUpdate();
	void Update();

	void SetFullScreen();

	bool GetKeyToggle(int key);

	std::map<int, float> Keys;

private:
	GameState* _state;
};