#pragma once
#include "IResource.hpp"
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SHADERPATH "Source/shaders/"
#define SCREENSHADERPATH "Source/shaders/ScreenShader/"

enum class Location
{
	L_MVP,
	L_M,
	L_TIME,
	L_ENABLE_TEXTURE,
	L_TEXTURE,
	L_MATERIAL,
	L_NORMAL_MAP,
	L_DIRECTIONAL,
	L_POINT,
	L_SPOT,
	L_COLOR,
};

class Shader : public IResource
{
public:
	Shader();
	~Shader();
	int Program = -1;
	int VertexShader;
	int FragmentShader;
	std::string Name;

	bool ScreenShader = false;

	// Set Vertex Shader.
	bool SetShader(std::string filename);
	// Set Fragment Shader.
	bool SetFragmentShader(std::string filename);

	// Link shader and set if is a screenShader.
	bool Link(bool screen = false);
	// Initialize the Shader
	void Initialize();
	// Reload Locations.
	int GetLocation(Location loc);

private:
	bool _linked = false;
	std::map<Location, int> _location;
};