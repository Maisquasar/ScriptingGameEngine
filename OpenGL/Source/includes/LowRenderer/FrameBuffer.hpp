#pragma once
#include "../Resources/ResourcesManager.hpp"
#include <glad/glad.h>
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Initialize(Int2D, ResourcesManager&);
	void Draw(Int2D size);

	Texture* Tex;
	ImColor ClearColor;

	Shader* shader;
	GLuint FBO;

private:

	GLuint _VBO;
	GLuint _RBO;
	GLuint _VAO;
	GLuint _Color;
};
