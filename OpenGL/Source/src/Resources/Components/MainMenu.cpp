#include "../Source/includes/Resources/Components/MainMenu.hpp"
#include "../Source/includes/Core/App.hpp"

void Menu::InitKeys()
{
	if (app.PlayerPrefs.GetInteger("Dirty") == 0) {
		Key["Forward"] = ImGuiKey_W;
		Key["Backward"] = ImGuiKey_S;
		Key["Left"] = ImGuiKey_A;
		Key["Right"] = ImGuiKey_D;
		Key["Jump"] = ImGuiKey_Space;
	}
	else
	{
		for (auto binds : KeysName)
		{
			Key[binds] = app.PlayerPrefs.GetInteger(binds);
		}
	}

}

void Menu::ShowSettings()
{
	int width, height;
	glfwGetWindowSize(app.Window, &width, &height);
	ImGui::SetWindowSize(ImVec2((float)width, (float)height - 20.f));
	ImGui::SetWindowPos(ImVec2(0, 20));
	int buttonsizeX = (width * 25 / 100);
	int buttonsizeY = height * 10 / 100;
	ImGui::SetWindowFontScale(5);
	int index = -1;
	ImGui::SetCursorPosY(0);
	for (auto&& keys : Key) {
		std::string Name = ImGui::GetKeyName(keys.second);
		index++;
		ImGui::SetCursorPosX((float)width / 2.f - (float)buttonsizeX / 2.f);
		ImGui::SetCursorPosY(30.f + (float)index * ((float)height / Key.size() - 50.f));
		// Start Button
		if (index == _selected)
			Name = "<>";
		if (ImGui::Button(Name.c_str(), ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)) || index == _selected)
		{
			_selected = index;
			auto all = ImGui::GetIO().KeysData;
			for (int i = 0; i < 617; i++)
			{
				if (all[i].DownDuration > 0.1f)
				{
					keys.second = ImGui::GetKeyIndex(i);
					_selected = -1;
				}
			}
		}
		ImGui::SameLine();
		ImGui::Text(keys.first.c_str());
	}
	ImGui::SetCursorPosX(0 + 200);
	ImGui::SetCursorPosY((float)height - 200.f);
	if (ImGui::Button("Back", ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)))
	{
		app.PlayerPrefs.SetInteger("Dirty", 1);
		for (auto keys : Key)
		{
			app.PlayerPrefs.SetInteger(keys.first, keys.second);
		}
		Type = Scene::S_MAIN;
	}
}

MainMenu::MainMenu() 
{
	ComponentName = "MainMenu";
}

MainMenu::~MainMenu()
{
}

void MainMenu::ShowComponent()
{
	ImGui::InputText("Load Scene", (char*)_sceneName, 64);
}
void Menu::Start()
{
	InitKeys();
}

void MainMenu::GameUpdate()
{
	int width, height;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Set window background to red
	if (ImGui::Begin("MainMenu", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration))
	{
		app.camera.input.MouseCaptured = false;
		glfwSetInputMode(app.Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwGetWindowSize(app.Window, &width, &height);
		ImGui::SetWindowSize(ImVec2((float)width, (float)height - 20.f));
		ImGui::SetWindowPos(ImVec2(0, 20));
		int buttonsizeX = (width * 25 / 100);
		int buttonsizeY = height * 10 / 100;
		ImGui::SetWindowFontScale(5);
		switch (Type)
		{
		case Scene::S_MAIN:
			ImGui::SetCursorPosX((float)width / 2.f - (float)buttonsizeX / 2.f);
			ImGui::SetCursorPosY(200);
			// Start Button
			if (ImGui::Button("Start", ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)))
			{
				app.camera.input.MouseCaptured = true;
				glfwSetInputMode(app.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				app.LoadScene(_sceneName);
			}
			ImGui::SetCursorPosX((float)width / 2.f - (float)buttonsizeX / 2.f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
			if (ImGui::Button("Settings", ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)))
			{
				Type = Scene::S_SETTINGS;
			}
			ImGui::SetCursorPosX((float)width / 2.f - (float)buttonsizeX / 2.f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);
			if (ImGui::Button("Quit", ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)))
			{
				app.CloseApp();
			}
			break;
		case Scene::S_SETTINGS: {
			ShowSettings();
			break;
		}
		default:
			break;
		}
		ImGui::End();
	}
	ImGui::PopStyleColor();

}

void MainMenu::Save(const std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"SceneName\" : \"%s\"\n", this->_sceneName);
#else
	line += string_format(space + "SceneName : %s\n", this->_sceneName);
#endif
}

MainMenu* MainMenu::Load(const char* data, uint32_t& pos)
{
	MainMenu* mm = new MainMenu();
	while (1)
	{
		if (data[pos] == 'S')
		{
			mm->SetScene(Loader::GetString(data, pos, 12));
			break;
		}
		else {
			pos++;
		}
	}
	return mm;
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_MainMenu, m) {
	py::class_<MainMenu, Component>(m, "MainMenu")
		.def("SetScene", &MainMenu::SetScene);
	m.def("CreateMainMenu", []() { return new MainMenu(); }, pybind11::return_value_policy::reference);

}