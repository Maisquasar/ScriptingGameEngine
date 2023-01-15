#include "../Source/includes/Resources/Components/PauseMenu.hpp"
#include "../Source/includes/Core/App.hpp"

PauseMenu::PauseMenu() 
{
	ComponentName = "PauseMenu";
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::ShowComponent()
{
	ImGui::InputText("Load Scene", (char*)_sceneName, 64);
}

void PauseMenu::GameUpdate()
{
	*GetPausePtr() = active;
	if (ImGui::IsKeyPressed(GLFW_KEY_P)) {
		active = !active;
		if (!active) {
			app.camera.input.MouseCaptured = true;
			glfwSetInputMode(app.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (active) {
		int width, height;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Set window background to red
		if (ImGui::Begin("PauseMenu", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration))
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
			{
				ImGui::SetCursorPosX(((float)width / 2.f) - (float)buttonsizeX / 2.f);
				ImGui::SetCursorPosY(200);
				ImGui::RenderText(ImVec2((float)width / 2.f - 80.f, 200.f), "Pause");
				ImGui::SetCursorPosX((float)width / 2.f - (float)buttonsizeX / 2.f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100);

				// Start Button
				if (ImGui::Button("Continue", ImVec2((float)width * 25.f / 100.f, (float)height * 10.f / 100.f)))
				{
					app.camera.input.MouseCaptured = true;
					glfwSetInputMode(app.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					active = false;
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
					app.PlayerPrefs.SetInteger("MainMenu", 1);

					app.camera.input.MouseCaptured = true;
					glfwSetInputMode(app.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					app.LoadScene(_sceneName);
				}
			}
			break;
			case Scene::S_SETTINGS: {
				ShowSettings();
				break;
			}
			}

			ImGui::End();
		}
		ImGui::PopStyleColor();
	}
}

void PauseMenu::Save(const std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"SceneName\" : \"%s\"\n", this->_sceneName);
#else
	line += string_format(space + "SceneName : %s\n", this->_sceneName);
#endif
}

PauseMenu* PauseMenu::Load(const char* data, uint32_t& pos)
{
	PauseMenu* mm = new PauseMenu();
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
PYBIND11_EMBEDDED_MODULE(S_PauseMenu, m) {
	py::class_<PauseMenu, Component>(m, "PauseMenu")
		.def("SetScene", &PauseMenu::SetScene);
	m.def("CreatePauseMenu", []() { return new PauseMenu(); }, pybind11::return_value_policy::reference);

}