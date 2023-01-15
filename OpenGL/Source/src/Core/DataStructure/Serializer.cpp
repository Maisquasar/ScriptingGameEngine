#include "../Source/includes/Core/DataStructure/Serializer.hpp"

Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

template <typename T> void Serializer::Save(std::string space, std::string& line, std::string Name, T value) {}
template <> void Serializer::Save<float>(std::string space, std::string& line, std::string Name, float value)
{
	line += space + Name + " : " + std::to_string(value) + "\n";
}
template <> void Serializer::Save<Vec3D>(std::string space, std::string& line, std::string Name, Vec3D value)
{
	line += space + Name + " : " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + "\n";
}
template <> void Serializer::Save<Vec2D>(std::string space, std::string& line, std::string Name, Vec2D value)
{
	line += space + Name + " : " + std::to_string(value.x) + " " + std::to_string(value.y) + "\n";
}
template <> void Serializer::Save<Vec4D>(std::string space, std::string& line, std::string Name, Vec4D value)
{
	line += space + Name + " : " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w) + "\n";
}
template <> void Serializer::Save <std::string>(std::string space, std::string& line, std::string Name, std::string value)
{
	line += space + Name + " : " + value + "\n";
}


template <typename T> void Serializer::AddArgument(std::string text, T* value) {}

template <> void Serializer::AddArgument<float>(std::string text, float* value)
{
	_argsf[text] = value;
}

template <> void Serializer::AddArgument<Vec3D>(std::string text, Vec3D* value)
{
	_argsv3[text] = value;
}

void Serializer::Search(const char* data, uint32_t& pos)
{
	while (data[pos] != '#')
	{
		auto text = Loader::GetString(data, pos, 0);
		for (auto fl : _argsf)
		{
			if (text == fl.first)
			{
				*fl.second = Loader::GetFloat(data, pos, 3);
				continue;
			}
		}
		for (auto v3 : _argsv3)
		{
			if (text == v3.first)
			{
				*v3.second = Loader::GetVec3(data, pos, 3);
				continue;
			}
		}
		for (auto v2 : _argsv2)
		{
			if (text == v2.first)
			{
				*v2.second = Loader::GetVec2(data, pos, 3);
				continue;
			}
		}
		for (auto v4 : _argsv4)
		{
			if (text == v4.first)
			{
				*v4.second = Loader::GetVec4(data, pos, 3);
				continue;
			}
		}
		for (auto str : _argss)
		{
			if (text == str.first)
			{
				*str.second = Loader::GetString(data, pos, 3);
				continue;
			}
		}
		pos++;
	}
}
