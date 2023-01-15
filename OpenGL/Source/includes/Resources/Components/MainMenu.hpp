#pragma once
class App;
#include "../Component.hpp"
#include "PlayerScript.hpp"

enum class Scene
{
	S_MAIN,
	S_SETTINGS
};

class Menu : public Component
{
public :
	Scene Type = Scene::S_MAIN;
	std::unordered_map<std::string, int> Key;
	void Start() override;
	void ShowSettings();
	virtual void InitKeys();
protected:
	int _selected = -1;
};

class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();

	void SetScene(std::string name) 
	{
		_sceneName[0] = '\0';
		for (int i = 0; i < name.size(); i++)
		{
			_sceneName[i] = name[i];
		}
	}

	void ShowComponent() override;
	void GameUpdate() override;
	void Save(std::string space, std::string& line) override;
	MainMenu* Load(const char* data, uint32_t& pos) override;

private:
	char _sceneName[64] = "";
};
