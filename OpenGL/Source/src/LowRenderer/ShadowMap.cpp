#include "../../includes/LowRenderer/ShadowMap.hpp"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Initialize(ResourcesManager& res)
{
	glGenFramebuffers(1, &depthMapFBO);

	Tex.Name = "ShadowMap";
	Tex.Index = res.TexId++;
	glActiveTexture(GL_TEXTURE0 + Tex.Index);
	glGenTextures(1, &Tex.TextureData);
	glBindTexture(GL_TEXTURE_2D, Tex.TextureData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution.x, resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glGenSamplers(1, &Tex.Sampler);
	glSamplerParameteri(Tex.Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(Tex.Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(Tex.Sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(Tex.Sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
	glBindTextureUnit(Tex.Index, Tex.TextureData);
	glBindSampler(Tex.Index, Tex.Sampler);
	Tex.Loaded = true;
	res.Resources["ShadowMap"] = new Texture(Tex);

	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glSamplerParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Draw(Int2D size)
{
	glViewport(0, 0, resolution.x, resolution.y);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}
