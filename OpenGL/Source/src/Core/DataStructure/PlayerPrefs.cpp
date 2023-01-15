#include "../Source/includes/Core/DataStructure/PlayerPrefs.hpp"

PlayerPrefs::PlayerPrefs(){}
PlayerPrefs::~PlayerPrefs(){}

void PlayerPrefs::Clear()
{
	_integers.clear();
	_floats.clear();
	_strings.clear();
}

void PlayerPrefs::SetInteger(std::string Name, int value)
{
	_integers[Name] = value;
}
void PlayerPrefs::SetFloat(std::string Name, float value)
{
	_floats[Name] = value;
}
void PlayerPrefs::SetString(std::string Name, std::string value)
{
	_strings[Name] = value;
}

int PlayerPrefs::GetInteger(std::string Name)
{
	return  _integers[Name] ? _integers[Name] : 0;
}
float PlayerPrefs::GetFloat(std::string Name)
{
	return _floats[Name] ? _integers[Name] : 0;
}
std::string PlayerPrefs::GetString(std::string Name)
{
	return _strings[Name];
}