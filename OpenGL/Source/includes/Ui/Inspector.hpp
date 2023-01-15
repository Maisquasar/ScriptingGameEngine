#pragma once
#include "UiWindow.hpp"
#include "../Core/DataStructure/Node.hpp"

#include "../Source/includes/LowRenderer/LightManager.hpp"
#include "../Source/includes/Resources/ResourcesManager.hpp"
#include "../Source/includes/Physic/CollisionManager.hpp"
#include "../Source/includes/LowRenderer/Camera.hpp"
#include "../Source/includes/Resources/Components/CameraLock.hpp"
#include "../Source/includes/Physic/Rigidbody.hpp"
#include "../Source/includes/Resources/Components/PlayerScript.hpp"
#include "../Source/includes/Resources/Components/MainMenu.hpp"
#include "../Source/includes/Resources/Components/PauseMenu.hpp"
#include "FileExplorer.hpp"
#include "../Source/includes/Resources/TxtFile.hpp"

class Inspector : public UiWindow
{
public:
	Inspector(GameState* gs) {  _state = gs; }
	~Inspector() {}

	Selectable* Selected = nullptr;
	Selectable* PrevSelected = nullptr;

	void Update(bool&);
	void MeshPopup(ResourcesManager&);
	void ChildWindow(ResourcesManager&);

	template <typename T> T* GetSelected() { return dynamic_cast<T*>(Selected); }

private:
	char _tmpName[64] = ""; // The Temporary name of the node selected
	GameState* _state;
};

