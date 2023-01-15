#pragma once
#include "../Core/Maths/Maths.hpp"
#include "../Resources/ResourcesManager.hpp"
#include <glad/glad.h>

//TODO: Finish Shadow Map.
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	Int2D resolution;
	GLuint depthMapFBO;
	GLuint depthMap;

	void Initialize(ResourcesManager&);
	void Draw(Int2D size);

	Texture Tex;
private:

};