#include "../Source/includes/Core/DataStructure/Inspector.hpp"
#include "../Source/includes/Core/App.hpp"

std::vector<bool> Change;
static Mesh sGlobal;
static bool sSelection[1] = { false };

void Inspector::Update(LightManager& lig, ResourcesManager& res, CollisionManager& col, bool& open, Camera& cam)
{
	if (!open)
		return;
	int number = 0;
	if (ImGui::Begin("Inspector", &open))
	{
		if (app.WindowChangeSize)
		{
			int width, height;
			glfwGetWindowSize(app.Window, &width, &height);
			ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowSize().x, 0));
			ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, height));
		}
		// Check if Selected
		if (Selected == nullptr) {
			ImGui::End();
			return;
		}
		if (auto selected = dynamic_cast<Node*>(Selected)) {
			// Check if Not same selected
			if (selected != PrevSelected)
			{
				_tmpName[0] = '\0';
				for (int i = 0; i < 64; i++)
					if (i <= selected->Name.size())
						_tmpName[i] = selected->Name[i];
				Change.clear();
				sGlobal = Mesh();
				PrevSelected = selected;
			}
			// Name Input
			if (ImGui::InputText("Name", _tmpName, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				selected->Name = _tmpName;
			}
			// Transform Component
			selected->transform.ShowComponent();
			// Show Components
			int index = -1;
			for (auto&& i : selected->Components)
			{
				index++;
				if (!i->windowOpen) {
					(void)std::remove(selected->Components.begin(), selected->Components.end(), i);
					break;
				}
				ImGui::PushID(index);
				ImGui::Checkbox("##", &i->enable);
				ImGui::SameLine();
				ImGui::Selectable(i->ComponentName.c_str(), false, 0, ImVec2(260, 0));

				int move_from = -1, move_to = -1;

				ImGuiDragDropFlags src_flags = 0;
				src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
				src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
				if (ImGui::BeginDragDropSource(src_flags))
				{
					if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
						ImGui::Text("Moving \"%s\"", i->ComponentName.c_str());
					ImGui::SetDragDropPayload("DND_DEMO_NAME", &index, sizeof(int));
					ImGui::EndDragDropSource();
				}

				if (ImGui::BeginDragDropTarget())
				{
					ImGuiDragDropFlags target_flags = 0;
					target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
					{
						move_from = *(const int*)payload->Data;
						move_to = index;

					}
					ImGui::EndDragDropTarget();
				}

				if (move_from != -1 && move_to != -1)
				{
					std::iter_swap(selected->Components.begin() + move_from, selected->Components.begin() + move_to);
					ImGui::SetDragDropPayload("DND_DEMO_NAME", &move_to, sizeof(int));
				}

				ImGui::SameLine();
				if (ImGui::CollapsingHeader("##", &i->windowOpen, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::TreePush();
					ImGui::BeginDisabled(!i->enable);
					i->ShowComponent();
					ImGui::EndDisabled();
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ChildWindow(res);

			ImGui::NewLine();
			// Add Component
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				static ImGuiTextFilter filter;
				filter.Draw();
				std::vector<std::string> lines;
				for (int i = 0; i < CData.Datas.size(); i++)
				{
					lines.push_back(CData.Datas[i]->ComponentName);
					if (filter.PassFilter(lines[i].c_str())) {
						if (ImGui::Button(CData.Datas[i]->ComponentName.c_str()))
						{
							bool canAdd = true;
							if (!CData.Datas[i]->multiple)
							{
								for (auto comp : selected->Components)
									if (comp->ComponentName == CData.Datas[i]->ComponentName)
										canAdd = false;
							}
							if (CData.Datas[i]->multiple || canAdd)
							{
								uint32_t pos;
								auto comp = CData.Datas[i];
								comp->gameObject = selected;
								selected->AddComponent(comp);
								ImGui::CloseCurrentPopup();
							}
						}
					}
				}
				ImGui::EndPopup();
			}
		}
		else if (auto file = GetSelected<File>())
		{
			if (selected != PrevSelected)
			{
				PrevSelected = selected;
			}
			if (file->Resource != nullptr)
			{
				ImGui::Image((ImTextureID)file->Tex->TextureData, ImVec2(64, 64));
				ImGui::SameLine();
				ImGui::SetWindowFontScale(2);
				ImGui::Text(file->Name.c_str());
				ImGui::Separator();
				ImGui::NewLine();
				ImGui::SetWindowFontScale(1);
				file->Resource->ShowResource();
			}
		}
	}
	ImGui::End();
}

void Inspector::ChildWindow(ResourcesManager& res)
{
	// Apply Change to All childs.
	if (auto selected = static_cast<Node*>(Selected)) {
		if (selected->children.size() > 0)
		{
			bool component = false;
			if (ImGui::TreeNode("Childrens")) {
				if (selected->IsComponentInside<Mesh>())
				{
					ImGui::Checkbox("##mesh", &sSelection[0]); // To enable/disable component.
					ImGui::SameLine();
					ImGui::BeginDisabled(!sSelection[0]);
					component = true;
					Change.resize(3, 0);
					sGlobal.GlobalMesh(Change);
					ImGui::EndDisabled();
				}
				//TODO: this with other components.
				ImGui::TreePop();
				if (component)
				{
					ImGui::Text("Apply to All Childs With This Component");
					if (ImGui::Button("Apply")) {
						if (sSelection[0]) {
							for (int i = 0; i < selected->children.size(); i++)
							{
								selected->children[i]->ReplaceComponent<Mesh>(sGlobal, Change);
							}
						}
					}
				}
			}
		}
	}
}
