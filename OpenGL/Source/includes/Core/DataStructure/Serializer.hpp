#pragma once
#include "../../Resources/SceneLoader.hpp"
#include <string>
#include <map>

class Serializer
{
public:
	Serializer();
	~Serializer();

	template<typename T> void AddArgument(std::string, T*);
	template<typename T> void Save(std::string space, std::string& line, std::string Name, T);

	void Search(const char* data, uint32_t& pos);
private:
	std::map<std::string, float*> _argsf;
	std::map<std::string, Vec2D*> _argsv2;
	std::map<std::string, Vec3D*> _argsv3;
	std::map<std::string, Vec4D*> _argsv4;
	std::map<std::string, std::string*> _argss;
};