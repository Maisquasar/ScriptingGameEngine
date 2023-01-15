#include "../../includes/Core/Input.hpp"
#include "../Source/includes/Core/App.hpp"

Input::Input() {}
Input::Input(GLFWwindow* window, GameState* state)
{
	Window = window;
	_state = state;
}

Input::~Input() {}


void Input::CameraUpdate()
{
	DeltaX = MouseDeltaX;
	DeltaY = MouseDeltaY;
	Forward = glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS;
	Backward = glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS;
	Right = glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS;
	Left = glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS;
	Up = glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS;
	Down = glfwGetKey(Window, GLFW_KEY_F) == GLFW_PRESS;
	Shift = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	Scroll = MouseScroll;
	MouseScroll = 0;
}

void Input::Update()
{
	double newMouseX, newMouseY;
	glfwGetCursorPos(Window, &newMouseX, &newMouseY);
	MouseDeltaX = (float)(newMouseX - MouseX);
	MouseDeltaY = (float)(newMouseY - MouseY);
	MouseX = newMouseX;
	MouseY = newMouseY;

	// Mouse Captured
	if (GetKeyToggle(GLFW_KEY_ESCAPE))
	{
		MouseCaptured = !MouseCaptured;
		if (!MouseCaptured)
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	// Wireframe
	if (GetKeyToggle(GLFW_KEY_F1) && *_state != GameState::GS_PLAY)
		Wireframe = !Wireframe;
	// FullScreen
	if (GetKeyToggle(GLFW_KEY_F11))
	{
		app.WindowChangeSize = true;
		FullScreen = !FullScreen;
		SetFullScreen();
	}
}

void Input::SetFullScreen()
{
	if (FullScreen)
	{
		if (_monitor == nullptr)
			_monitor = glfwGetPrimaryMonitor();
		// backup window position and window size
		glfwGetWindowPos(Window, &_wndPos[0], &_wndPos[1]);
		glfwGetWindowSize(Window, &_wndSize[0], &_wndSize[1]);

		// get resolution of monitor
		const GLFWvidmode* mode = glfwGetVideoMode(_monitor);

		// switch to full screen
		glfwSetWindowMonitor(Window, _monitor, 0, 0, mode->width, mode->height, 0);
	}
	else
	{

		// restore last window size and position
		glfwSetWindowMonitor(Window, nullptr, _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0);
	}

	MouseCaptured = false;
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::GetKeyToggle(int key)
{
	if (glfwGetKey(Window, key) == GLFW_PRESS && Keys[key] <= 0)
	{
		Keys[key] = 0.5;
		return true;
	}
	else if (glfwGetKey(Window, key) == GLFW_RELEASE && Keys[key] > 0)
	{
		Keys[key] -= ImGui::GetIO().DeltaTime * 5;
	}
	return false;
}

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Input, m) {
	py::class_<Input>(m, "Input")
		.def_readwrite("Scroll", &Input::Scroll)
		.def_readwrite("Up", &Input::Up)
		.def_readwrite("Down", &Input::Down)
		.def_readwrite("Backward", &Input::Backward)
		.def_readwrite("Forward", &Input::Forward)
		.def_readwrite("Right", &Input::Right)
		.def_readwrite("Left", &Input::Left)
		.def_readwrite("Shift", &Input::Shift)
		.def_readwrite("DeltaX", &Input::DeltaX)
		.def_readwrite("DeltaY", &Input::DeltaY)
		.def_readwrite("MouseCaptured", &Input::MouseCaptured)
		.def("Update", &Input::Update)
		.def("CameraUpdate", &Input::CameraUpdate);
}