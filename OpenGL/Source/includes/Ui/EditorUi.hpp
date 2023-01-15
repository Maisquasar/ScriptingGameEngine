#pragma once
#include "LoadingWindow.hpp"
#include "FileExplorer.hpp"
#include "Inspector.hpp"
static const char* sThemes[] = { "Default", "Classic", "Light", "Customize" };


class EditorUi
{
public:
	Inspector inspector = Inspector(new GameState());
	FileExplorer fileExplorer;
	bool FullyLoaded = false;

	EditorUi();
	~EditorUi();

	void Start();

	void DrawUi();

	void ShowAll();

	void HideOnPlay();

	void ModelPopup(Node*);
	void MainMenuBar();
	void ShowHierarchy();
	void ShowConfig();
	void ShowTheme();
	void ShowRightClick();
	void OpenRightClick(Node*);

	void SetThemeId(int id) { _themeId = id; }
	int GetThemeId() { return _themeId; }
private:
	LoadingWindow _loadingWindow;

	// Windows boolean for open/close.
	bool _openInspector = true;
	bool _openHierarchy = true;
	bool _openConfig = true;
	bool _openConsole = true;
	bool _openThemes = true;
	bool _openFileExplorer = true;
	char _sceneName[32] = "";
	int _themeId = 0;


	float LastRecord = 0.f; // Last record time.
	std::vector<float> Fps; // Vector of Fps.
	std::vector<float> Ms; // Vector of Ms.
};