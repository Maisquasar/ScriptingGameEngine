#include "../Source/includes/Resources/Texture.hpp"
#include "../../includes/Core/App.hpp"
#include <STB_Image/stb_image.h>

Texture::Texture()
{}
Texture::Texture(int index)
{
	this->Index = index;
}

Texture::~Texture()
{
	if (Sampler != 0)
		glDeleteSamplers(1, &Sampler);
}

void Texture::Bind()
{
	glGenSamplers(1, &Sampler);

	glSamplerParameteri(Sampler, GL_TEXTURE_MIN_FILTER, Filter);
	glSamplerParameteri(Sampler, GL_TEXTURE_MAG_FILTER, Filter);

	glSamplerParameteri(Sampler, GL_TEXTURE_WRAP_S, Wrapping);
	glSamplerParameteri(Sampler, GL_TEXTURE_WRAP_T, Wrapping);

	glSamplerParameterf(Sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
	glBindTextureUnit(Index, TextureData);
	glBindSampler(Index, Sampler);
	Loaded = true;
}

void Texture::LoadTexture(const std::string Path)
{
	this->Name = Path;
	int NrChannels;
	data = stbi_load(Path.c_str(), &Width, &Height, &NrChannels, 4);
	if (data == nullptr) {
		LOG(LogType::L_ERROR, "Can't load Texture: \"%s\"", Path.c_str());
		return;
	}
	SemiLoaded = true;
}

void Texture::Initialize()
{
	glActiveTexture(GL_TEXTURE0 + Index);
	glGenTextures(1, &TextureData);
	glBindTexture(GL_TEXTURE_2D, TextureData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	LOG(LogType::INFO, "Successfully loaded Texture: %s With Id : %d", Name.c_str(), Index);
	Bind();
}

void Texture::SetFilter(const int filter)
{
	this->Filter = filter;
	this->Bind();
}
void Texture::SetWrapping(const int Wrapping)
{
	this->Wrapping = Wrapping;
	this->Bind();
}


void Texture::ShowImGui() {}

#pragma region Load/Save

void Texture::Save(const std::string space, std::string& line)
{
	line += space + string_format("\"Name\": \"%s\",\n", this->Name.c_str());
	line += space + string_format("\"Filter\": %d,\n", this->Filter);
	line += space + string_format("\"Wrapping\": %d\n", this->Wrapping);
}

Texture* Texture::Load(const char* data, uint32_t& pos)
{
	std::string T_Name = Loader::GetString(data, pos, 8);
	Texture* tex = new Texture();
	if (Texture* tmp = app.GetResourcesManager()->Get<Texture*>(T_Name))
		tex = tmp;
	else
		LOG(LogType::L_ERROR, "Can't load Texture: \"%s\"", T_Name.c_str());
	while (1)
	{
		if (data[pos] == 'F')
		{
			tex->Filter = Loader::GetInt(data, pos, 9);
		}
		else if (data[pos] == 'W')
		{
			tex->Wrapping = Loader::GetInt(data, pos, 11);
			break;
		}
		else {
			pos++;
		}
	}
	return tex;
}

#pragma endregion


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Texture, m) {
	py::class_<Texture>(m, "Texture")
		.def_readwrite("Name", &Texture::Name)
		.def_readwrite("Filter", &Texture::Filter)
		.def_readwrite("Wrapping", &Texture::Wrapping);
}