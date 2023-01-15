#pragma once
class App;
#include "../Component.hpp"
#include "MainMenu.hpp"
#include "PlayerScript.hpp"

class PauseMenu : public Menu
{
public:
	PauseMenu();
	~PauseMenu();

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
	PauseMenu* Load(const char* data, uint32_t& pos) override;
	bool active = false;

private:
	char _sceneName[64] = "";
};
