#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../Resources/Component.hpp"
#include "../Resources/SceneLoader.hpp"

#define DIFFUSE Vec4D  
#define AMBIENT Vec4D  
#define SPECULAR Vec4D  

class Light : public Component
{
public:
	int Index = -1;

	Vec4D Diffuse;
	Vec4D Ambient;
	Vec4D Specular;
	Vec3D Position;

	float AmbientStrenght = 0.1f;
	float SpecularStrenght = 0.5f;

	bool Dirty = true;

	Light();
	Light(Vec4D dif, Vec4D amb, Vec4D spe);
	~Light();

	virtual void Update(GLuint shader) {};

	void SetLight(Vec4D dif, Vec4D amb, Vec4D spe);

	void Save(std::string space, std::string& line) override {}
protected:
};

class PointLight : public Light
{
public:
	float Constant = 0.5f;
	float Linear = 0.5f;
	float Quadratic = 0.5f;

	PointLight();
	PointLight(int index);
	PointLight(Vec4D dif, Vec4D amb, Vec4D spe, int index);
	~PointLight();

	void Update(GLuint shader) override;
	void Start() override;

	float* GetValue();
	void UpdateValues();

	void SetComponent(float cst, float lin, float qua);
	void SetComponent(Vec3D parameters);

	void ShowComponent() override;

	void Save(std::string space, std::string& line) override;
	PointLight* Load(const char* data, uint32_t& pos) override;
private:
	float _values[21];
};

class DirectionalLight : public Light
{
public:
	Vec3D Direction;
	DirectionalLight();
	DirectionalLight(int index);
	DirectionalLight(Vec4D dif, Vec4D amb, Vec4D spe, int index);
	~DirectionalLight();
	void Start() override;
	void Update(GLuint shader) override;
	void ShowComponent() override;

	float* GetValue();
	void UpdateValues();

	void Save(std::string space, std::string& line) override;
	DirectionalLight* Load(const char* data, uint32_t& pos) override;
private:
	float _values[21];
};

class SpotLight : public Light
{
public:
	float CutOff = 0.7f;
	float OuterCutOff = 0.75f;
	Vec3D Direction;

	float Constant = 0.2f;
	float Linear = 0.2f;
	float Quadratic = 0.2f;

	SpotLight();
	SpotLight(int index);
	SpotLight(Vec4D dif, Vec4D amb, Vec4D spe, int index);
	~SpotLight();

	void Start() override;
	void Update(GLuint shader) override;

	float* GetValue();
	void UpdateValues();

	void ShowComponent() override;

	void Save(std::string space, std::string& line) override;
	SpotLight* Load(const char* data, uint32_t& pos) override;
private:
	float _values[26];
};