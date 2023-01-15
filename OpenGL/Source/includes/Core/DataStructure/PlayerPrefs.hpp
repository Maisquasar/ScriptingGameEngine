#pragma once
#include <map>
#include <string>
// PlayerPrefs is a Global variable to stock Variable like string, int and float.
class PlayerPrefs
{
public:
	PlayerPrefs();
	~PlayerPrefs();

	void Clear();
		
	void SetInteger(std::string Name, int);
	void SetFloat(std::string Name, float);
	void SetString(std::string Name, std::string);

	int GetInteger(std::string Name);
	float GetFloat(std::string Name);
	std::string GetString(std::string Name);
private:
	std::map<std::string, int> _integers;
	std::map<std::string, float> _floats;
	std::map<std::string, std::string> _strings;
};