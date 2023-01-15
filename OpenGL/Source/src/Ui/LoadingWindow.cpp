#include "../Source/includes/Ui/LoadingWindow.hpp"
#include "../Source/includes/Core/App.hpp"
LoadingWindow::LoadingWindow()
{
}

LoadingWindow::~LoadingWindow()
{
}

void LoadingWindow::Show()
{
	if (!FullyLoaded) {
		ImVec2 Size = ImVec2(200, 25);
		auto WindowSize = app.GetWindowSize();
		ImGui::SetNextWindowSize(Size);
		ImGui::SetNextWindowPos(ImVec2(WindowSize.x - Size.x, 0));
		if (ImGui::Begin("Loading", (bool*)true, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav))
		{
			if (!_window)
			{
				_window = ImGui::GetCurrentWindow();
			}
			if (app.WindowChangeSize) {
				ImGui::SetWindowFocus();
			}
			int Loaded = 0;
			for (auto i : app.GetResourcesManager()->Resources)
			{
				if (i.second)
					if (i.second->IsLoaded()) {
						Loaded++;
						continue;
					}
			}
			float progress = (float)((float)Loaded / (float)app.GetResourcesManager()->Resources.size()) * 100;
			ImGui::Text("Loading");
			ImGui::SameLine();
			char buf[12];
			sprintf_s(buf, 12, "%.2f %%", progress);
			ImGui::ProgressBar((progress / 100.f), ImVec2(0.0f, 0.0f), buf);
			if (Loaded == app.GetResourcesManager()->Resources.size()) {
				LOG(LogType::INFO, "Everythings has been load !");
				FullyLoaded = true;
				auto end = std::chrono::high_resolution_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - app.LoadingTime);
				LOG(LogType::INFO, "Loading Time : %.5f seconds.\n", elapsed.count() * 1e-9);
			}
		}
		ImGui::End();
	}
}