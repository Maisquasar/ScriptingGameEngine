#include "../Source/includes/Ui/EditorUi.hpp"
#include "../Source/includes/Core/App.hpp"

EditorUi::EditorUi()
{
}

EditorUi::~EditorUi()
{
}

void EditorUi::Start()
{
	inspector = Inspector(&app.State);
	fileExplorer.Refresh();
}

void EditorUi::ShowAll()
{
	_openConfig = true;
	_openConsole = true;
	_openHierarchy = true;
	_openInspector = true;
}

void EditorUi::HideOnPlay()
{
	_openConfig = false;
	_openConsole = false;
	_openHierarchy = true;
	_openInspector = true;
}

static Node* cur;
static Node sCopyClipBoard;
void EditorUi::DrawUi()
{
	MainMenuBar();
	inspector.Update(_openInspector);
	fileExplorer.ShowImGui(_openFileExplorer);
	Console.DrawConsole(_openConsole);

	ShowConfig();
	ShowHierarchy();
	ShowTheme();
	ShowRightClick();
#if MULTITHREAD
	_loadingWindow.Show();
#endif
	if ((ImGui::IsKeyDown(ImGuiKey_RightCtrl) || ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) && ImGui::IsKeyPressed(ImGuiKey_C) && inspector.GetSelected<Node>() != nullptr)
	{
		// Copy
		sCopyClipBoard = *inspector.GetSelected<Node>();
		app.CopyToClipboard(inspector.GetSelected<Node>());
	}
	// Ctrl + X : cut
	if ((ImGui::IsKeyDown(ImGuiKey_RightCtrl) || ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) && ImGui::IsKeyPressed(ImGuiKey_X) && inspector.GetSelected<Node>() != nullptr)
	{
		// Copy
		sCopyClipBoard = *inspector.GetSelected<Node>();
		app.CopyToClipboard(inspector.GetSelected<Node>());

		// Delete Node.
		inspector.GetSelected<Node>()->Delete();
		inspector.Selected = nullptr;
	}
	// Ctrl + V : paste
	if ((ImGui::IsKeyDown(ImGuiKey_RightCtrl) || ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) && ImGui::IsKeyPressed(ImGuiKey_V))
	{
		Node* tmp = Node::LoadScene("Clipboard.txt", app.GetResourcesManager()->Resources);
		if (app.GetLightManager()->Add(tmp))
			inspector.GetSelected<Node>()->addChild(tmp);

		tmp->SetWorldPosition(sCopyClipBoard.transform.GetWorldPosition());
		tmp->SetWorldRotation(sCopyClipBoard.transform.GetWorldRotation());
		tmp->SetWorldScale(sCopyClipBoard.transform.GetWorldScale());
	}
	// SHIFT + Ctrl + F : Go To camera.
	// Set The Position and rotation To camera.
	if ((ImGui::IsKeyDown(ImGuiKey_RightShift) || ImGui::IsKeyDown(ImGuiKey_LeftShift)) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_F) && inspector.GetSelected<Node>() != nullptr)
	{
		inspector.GetSelected<Node>()->SetWorldRotation(Vec3D(app.camera.rotation.y, app.camera.rotation.x - 180, 0));
		inspector.GetSelected<Node>()->SetWorldPosition(app.camera.position);
	}

	// When Select a new Node, Camera focus go to Node world position.
	if (app.settings.GoToNode && inspector.GetSelected<Node>() != cur && inspector.GetSelected<Node>() != nullptr)
	{
		cur = inspector.GetSelected<Node>();
		app.camera.focus = cur->transform.GetWorldPosition();
		// Update 1 Frame To replace the camera.
		app.camera.Update(ImGui::GetIO().DeltaTime, app.State == GameState::GS_PLAY);
	}
}

void EditorUi::ModelPopup(Node* node)
{
	if (ImGui::BeginPopupModal("Models"))
	{
		// Search Bar
		static ImGuiTextFilter filter;
		filter.Draw();
		std::vector<std::string> lines;

		std::vector<Model3D> tmp;
		app.GetResourcesManager()->GetAllModels(tmp);
		ImGui::BeginGroup();
		for (int i = 0; i < tmp.size(); i++)
		{
			lines.push_back(tmp[i].Name);
			if (filter.PassFilter(lines[i].c_str())) {
				if (ImGui::Button(tmp[i].Name.c_str()))
				{
					Model3D* model = app.GetResourcesManager()->Get<Model3D*>(tmp[i].Name);
					Node* m = app.GetLightManager()->CopyNode(model);
					m->transform.SetLocalTransform(Vec3D(0), Vec3D(0), Vec3D(1));
					node->addChild(m);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndGroup();
		ImGui::Separator();
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void EditorUi::MainMenuBar()
{
	bool popup = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Inspector", &_openInspector);
			ImGui::Checkbox("Hierarchy", &_openHierarchy);
			ImGui::Checkbox("Config", &_openConfig);
			ImGui::Checkbox("Console", &_openConsole);
			ImGui::Checkbox("Themes", &_openThemes);
			ImGui::Checkbox("File Explorer", &_openFileExplorer);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::Button("Camera Settings"))
			{
				// Input Settings Popup
				ImGui::OpenPopup("Camera");
				ImGui::SetNextWindowSize(ImVec2(700, 400));
			}
			if (ImGui::BeginPopupModal("Camera"))
			{
				ImGui::DragFloat("Camera Speed", &app.camera.speed, 0.05f, 0, FLT_MAX);
				ImGui::DragFloat("FOV", &app.camera.fov, 0.05f, 0, FLT_MAX);
				ImGui::DragFloat("Near", &app.camera.Near, 0.05f, 0, FLT_MAX);
				ImGui::DragFloat("Far", &app.camera.Far, 0.05f, 0, FLT_MAX);
				ImGui::Checkbox("Go To Node on Click", &app.settings.GoToNode);
				if (ImGui::Button("Save"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::Button("Model 3D"))
			{
				ImGui::OpenPopup("Models");
			}
			ModelPopup(app.SceneNode);
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::Button("Point Light"))
				{
					auto light = new Node();
					if (app.GetLightManager()->AddLightComponent<PointLight>(light)) {
						light->Name = "Point Light " + std::to_string(dynamic_cast<PointLight*>(light->Components.back())->Index);
						app.SceneNode->addChild(light);
					}
				}
				if (ImGui::Button("Directional Light"))
				{
					auto light = new Node();
					if (app.GetLightManager()->AddLightComponent<DirectionalLight>(light)) {
						light->Name = "Directional Light " + std::to_string(dynamic_cast<DirectionalLight*>(light->Components.back())->Index);
						app.SceneNode->addChild(light);
					}
				}
				if (ImGui::Button("SpotLight"))
				{
					auto light = new Node();
					if (app.GetLightManager()->AddLightComponent<SpotLight>(light)) {
						light->Name = "SpotLight " + std::to_string(dynamic_cast<SpotLight*>(light->Components.back())->Index);
						app.SceneNode->addChild(light);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::InputText("Scene Name", _sceneName, 32);
			if (ImGui::Button("Save"))
			{
				app.SaveScene(_sceneName);
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				app.LoadScene(_sceneName);
			}
			ImGui::SameLine();
			if (ImGui::Button("New"))
			{
				app.LoadScene("Default");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("FrameBuffer"))
		{
			if (ImGui::BeginMenu("Shaders"))
			{
				for (auto resources : app.GetResourcesManager()->Resources)
				{
					if (auto shader = dynamic_cast<Shader*>(resources.second))
					{
						if (shader->ScreenShader) {
							if (ImGui::Button(shader->R_Name.c_str()))
							{
								app.frameBuffer.shader = shader;
							}
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::ColorEdit4("Clear Color", &app.frameBuffer.ClearColor.Value.x);
			if (ImGui::Button("FrameBuffer Texture"))
			{
				ImGui::OpenPopup("Texture");
			}
			if (auto tex = Mesh::TexturePopup("Texture"))
				app.frameBuffer.Tex = tex;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close"))
		{
			if (ImGui::Button("Close App"))
				app.CloseApp();
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Play"))
		{
			if (app.State != GameState::GS_PLAY)
				app.BeginPlay();
			else
				app.EndPlay();
		}
		ImGui::EndMainMenuBar();
	}
	if (ImGui::IsKeyPressed(ImGuiKey_F5))
	{
		if (app.State != GameState::GS_PLAY)
			app.BeginPlay();
		else
			app.EndPlay();
	}
}

void EditorUi::ShowHierarchy()
{
	if (!_openHierarchy)
		return;
	bool deleted = false;
	auto lastSelected = inspector.GetSelected<Node>();
	if (ImGui::Begin("Hierarchy", &_openHierarchy))
	{
		if (app.WindowChangeSize)
		{
			auto Size = app.GetWindowSize();

			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, Size.y));
		}

		ImGui::BeginChild("Child");
		app.SceneNode->ShowImGui(lastSelected, 0);
		if (lastSelected != nullptr) {
			if (ImGui::IsKeyPressed(GLFW_KEY_DELETE, false) && ImGui::IsWindowFocused())
			{
				deleted = true;
				if (lastSelected->IsComponentInside<CameraLock>())
					app.camera.Lockposition = nullptr;
				lastSelected->Delete();
			}
		}
		ImGui::EndChild();
	}
	if (lastSelected)
		inspector.PrevSelected = inspector.Selected;
	if (lastSelected)
		inspector.Selected = deleted ? nullptr : lastSelected;
	ImGui::End();
}

void EditorUi::ShowConfig()
{
	if (!_openConfig)
		return;
	if (ImGui::Begin("Config", &_openConfig))
	{
		//Fps Graph
		if (LastRecord < app.TimeSinceStart - 0.1)
		{
			LastRecord = app.TimeSinceStart;
			if (Fps.size() < 60) {
				Fps.push_back(1 / ImGui::GetIO().DeltaTime);
				Ms.push_back(ImGui::GetIO().DeltaTime);
			}
			else {
				Fps.erase(Fps.begin());
				Fps.push_back(1 / ImGui::GetIO().DeltaTime);
				Ms.erase(Ms.begin());
				Ms.push_back(ImGui::GetIO().DeltaTime);
			}
		}
		if (Fps.size() > 0) {
			ImGui::PlotLines("MS", &Ms[0], (int)Ms.size(), 0, NULL, 0.0f, FLT_MAX, ImVec2(0, 40));
			auto max = *std::max_element(std::begin(Fps), std::end(Fps));
			auto min = *std::min_element(std::begin(Fps), std::end(Fps));
			char txt[32];
			snprintf(txt, 32, "Max Fps : %.2f |", max);
			ImGui::TextColored(ImVec4(0.f, 9.f, 0.f, 1.f), txt);
			ImGui::SameLine();
			snprintf(txt, 32, "Min Fps : %.2f", min);
			ImGui::TextColored(ImVec4(0.9f, 0.f, 0.f, 1.f), txt);
		}
		ImGui::Checkbox("Vsync", &app.settings.Vsync);
		ImGui::Checkbox("Show Collider", &app.settings.ShowCollision);
		char txt[32];
		auto Size = app.GetWindowSize();
		snprintf(txt, 32, "Width : %d | Height : %d", Size.x, Size.y);
		ImGui::Text(txt);
		int Tris = 0;
		for (auto& node : app.SceneNode->GetAllChildrens())
		{
			for (auto& comp : node->Components)
			{
				if (auto mesh = dynamic_cast<Mesh*>(comp))
				{
					if (!mesh->enable)
						break;
					Tris += (int)mesh->Indices.size();
				}
			}
		}
		snprintf(txt, 32, "Triangles Draw : %d", Tris);
		ImGui::Text(txt);

		if (ImGui::TreeNode("Other Informations "))
		{
			snprintf(txt, 32, "OpenGL Version : %s", glGetString(GL_VERSION));
			ImGui::Text(txt);

			snprintf(txt, 32, "Vendor : %s", glGetString(GL_VENDOR));
			ImGui::Text(txt);

			snprintf(txt, 32, "GPU : %s", glGetString(GL_RENDERER));
			ImGui::Text(txt);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void SetupImGuiStyle()
{// Sorry if there's any errors here, I translated this back by hand.
	auto& io = ImGui::GetIO();

	auto& style = ImGui::GetStyle();
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 0.0f;
	style.PopupBorderSize = 0.0f;
	style.GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void EditorUi::ShowTheme()
{
	if (!_openThemes)
		return;
	if (ImGui::Begin("Theme", &_openThemes))
	{
		if (ImGui::BeginCombo("Themes", sThemes[_themeId]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(sThemes); n++)
			{
				const bool is_selected = (_themeId == n);
				if (ImGui::Selectable(sThemes[n], is_selected)) {
					_themeId = n;
					switch (_themeId)
					{
					case 0: ImGui::StyleColorsDark(); break;
					case 1: ImGui::StyleColorsClassic(); break;
					case 2: ImGui::StyleColorsLight(); break;
					case 3: SetupImGuiStyle(); break;
					}
				}
			}
			ImGui::EndCombo();
		}

	}
	ImGui::End();
}

static Node* sRightClicked;
void EditorUi::ShowRightClick() {
	for (auto node : app.SceneNode->GetAllChildrens()) {
		if (node->IsHovered() && ImGui::GetIO().MouseClicked[1] && !ImGui::IsPopupOpen("RightClick")) {
			ImGui::OpenPopup("RightClick");
			sRightClicked = node;
		}
	}
	OpenRightClick(sRightClicked);
}

void EditorUi::OpenRightClick(Node* node)
{
	if (ImGui::BeginPopup("RightClick"))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::Button("Model 3D"))
			{
				ImGui::OpenPopup("Models");
			}
			ModelPopup(node);
			if (ImGui::Button("Node"))
			{
				auto nodes = new Node();
				nodes->Name = "Node";
				node->addChild(nodes);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::Button("Point Light"))
				{
					auto light = new Node();
					light->AddComponent(new PointLight());
					if (app.GetLightManager()->CanBeAdd<PointLight>(light)) {
						node->AddComponent(new PointLight());
						node->Name = "Point Light " + std::to_string(dynamic_cast<DirectionalLight*>(light->Components.back())->Index);
						node->addChild(light);
					}
				}
				if (ImGui::Button("Directional Light"))
				{
					auto light = new Node();
					light->AddComponent(new DirectionalLight());
					if (app.GetLightManager()->CanBeAdd<DirectionalLight>(light)) {
						node->AddComponent(new DirectionalLight());
						light->Name = "Directional Light " + std::to_string(dynamic_cast<DirectionalLight*>(light->Components.back())->Index);
						node->addChild(light);
					}
				}
				if (ImGui::Button("Spot Light"))
				{
					auto light = new Node();
					light->AddComponent(new SpotLight());
					if (app.GetLightManager()->CanBeAdd<SpotLight>(light)) {
						node->AddComponent(new SpotLight());
						light->Name = "Spot Light " + std::to_string(dynamic_cast<SpotLight*>(light->Components.back())->Index);
						node->addChild(light);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}


