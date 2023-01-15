#pragma once
#include "../Core/Maths/Maths.hpp"
class IResource;
#include "IResource.hpp"
#include "Texture.hpp"

class Material : public IResource
{
public:
	Material() {
		Name = "Default";
		_values[0] = this->Ambient.x;
		_values[1] = this->Ambient.y;
		_values[2] = this->Ambient.z;
		_values[3] = this->Ambient.w;
		_values[4] = this->Diffuse.x;
		_values[5] = this->Diffuse.y;
		_values[6] = this->Diffuse.z;
		_values[7] = this->Diffuse.w;
		_values[8] = this->Specular.x;
		_values[9] = this->Specular.y;
		_values[10] = this->Specular.z;
		_values[11] = this->Specular.w;
		_values[12] = this->Shininess;
		Dirty = true;
	};
	~Material() {};

	std::string Name;

	Vec4D Ambient = Vec4D(1);
	Vec4D Diffuse = Vec4D(1);
	Vec4D Specular = Vec4D(1);
	Texture NormalMap;
	float Shininess = 32;

	bool Dirty = true;

	Texture* Tex = nullptr;
	std::string TexName;

	void Save(std::string space, std::string& line);
	static Material Load(const char* data, uint32_t& pos);

	void Update()
	{
		if (!Dirty)
			return;
		_values[0] = this->Ambient.x;
		_values[1] = this->Ambient.y;
		_values[2] = this->Ambient.z;
		_values[3] = this->Ambient.w;
		_values[4] = this->Diffuse.x;
		_values[5] = this->Diffuse.y;
		_values[6] = this->Diffuse.z;
		_values[7] = this->Diffuse.w;
		_values[8] = this->Specular.x;
		_values[9] = this->Specular.y;
		_values[10] = this->Specular.z;
		_values[11] = this->Specular.w;
		_values[12] = this->Shininess;
		Dirty = false;

	}

	float* GetValues()
	{
		return _values;
	}

	void SetNormalMap(Texture* tex)
	{
		if (tex)
			this->NormalMap = *tex;
	}
private:
	float _values[13];

};