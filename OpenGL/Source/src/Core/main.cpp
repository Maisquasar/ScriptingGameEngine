#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB_Image/stb_image.h>

#include <iostream>
#include <vector>
#include "../Source/includes/Core/App.hpp"

void processInput(GLFWwindow* window);

AppInitializer datas = {
	1600, 900, "Plateformer GL"
};

int main()
{
	app.InitApp(datas);

	app.Update();

	app.ClearApp();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}