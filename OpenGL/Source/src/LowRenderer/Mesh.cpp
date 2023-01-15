#include "../Source/includes/LowRenderer/Mesh.hpp"
#include "../Source/includes/Core/App.hpp"
#include <STB_Image/stb_image.h>

Mesh::Mesh()
{
	multiple = false;
	ComponentName = "Mesh";
	if (auto shad = app.GetResourcesManager()->Get<Shader*>("Source/shaders/DefaultShader"))
		CurrentShader = shad;
}

Mesh::~Mesh()
{
	if (_VAO != 0)
		glDeleteVertexArrays(1, &_VAO);
	if (_VBO != 0)
		glDeleteBuffers(1, &_VBO);
}

void MeshPopup(Mesh* mesh);
void Mesh::Loop(std::vector<unsigned int>& indices, std::vector<float>& vertices)
{
	std::map<int, Vec3D> Tangents;
	for (int k = 0; k < Indices.size(); k += 3)
	{
		Vec3D Edge1 = Positions[Indices[k + (size_t)1].x] - Positions[Indices[k].x];
		Vec3D Edge2 = Positions[Indices[k + (size_t)2].x] - Positions[Indices[k].x];

		float DeltaU1 = TextureUVs[Indices[k + (size_t)1].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV1 = TextureUVs[Indices[k + (size_t)1].y].y - TextureUVs[Indices[k].y].y;
		float DeltaU2 = TextureUVs[Indices[k + (size_t)2].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV2 = TextureUVs[Indices[k + (size_t)2].y].y - TextureUVs[Indices[k].y].y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		Vec3D Tangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Tangents[k] = Tangents[k] + Tangent;
		Tangents[k + 1] = Tangents[k] + Tangent;
		Tangents[k + 2] = Tangents[k] + Tangent;
	}
	for (int k = 0; k < Indices.size(); k++)
	{
		vertices.push_back(Positions[Indices[k].x].x);
		vertices.push_back(Positions[Indices[k].x].y);
		vertices.push_back(Positions[Indices[k].x].z);

		vertices.push_back(Normals[Indices[k].z].x);
		vertices.push_back(Normals[Indices[k].z].y);
		vertices.push_back(Normals[Indices[k].z].z);

		vertices.push_back(TextureUVs[Indices[k].y].x);
		vertices.push_back(TextureUVs[Indices[k].y].y);

		vertices.push_back(Tangents[k].x);
		vertices.push_back(Tangents[k].y);
		vertices.push_back(Tangents[k].z);
	}
}
void Mesh::Start()
{
	if (Vertices.empty()) {
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		Loop(_indices, _vertices);

		this->Vertices = _vertices;
	}
#if MULTITHREAD
	if (std::this_thread::get_id() == app.MainThreadId)
		Initialize();
#else
	Initialize();
#endif
}

void Mesh::Initialize()
{
	if (_loaded)
		return;

	if (Vertices.empty()) {
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		Loop(_indices, _vertices);

		this->Vertices = _vertices;
	}

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	//glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if (!Vertices.empty()) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(POS);
		// normal attribute
		glVertexAttribPointer(NML, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(NML);
		// texture coord attribute
		glVertexAttribPointer(TUV, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[6])));
		glEnableVertexAttribArray(TUV);

		glVertexAttribPointer(TAN, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[8])));
		glEnableVertexAttribArray(TAN);
	}
	_loaded = true;
}

void Mesh::Draw(Mat4D MVP, bool outline)
{
	if (!_loaded || CurrentShader == nullptr)
		return;
	CurrentMaterial.Update();
	glUseProgram(CurrentShader->Program);
	glUniformMatrix4fv(CurrentShader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniformMatrix4fv(CurrentShader->GetLocation(Location::L_M), 1, GL_TRUE, &GetTransform()->GetModelMatrix().content[0][0]);
	glUniform1f(CurrentShader->GetLocation(Location::L_TIME), (float)glfwGetTime());

	if (outline) {
		glUniform1i(CurrentShader->GetLocation(Location::L_ENABLE_TEXTURE), false);
		glUniform4f(CurrentShader->GetLocation(Location::L_COLOR), 1, 0.5, 0.1, 1);
	}
	else if (CurrentTexture != nullptr) {
		glUniform1i(CurrentShader->GetLocation(Location::L_ENABLE_TEXTURE), true);
		glUniform1i(CurrentShader->GetLocation(Location::L_TEXTURE), CurrentTexture->Index);
	}
	else
	{
		glUniform1i(CurrentShader->GetLocation(Location::L_ENABLE_TEXTURE), false);
	}

	glUniform1fv(CurrentShader->GetLocation(Location::L_MATERIAL), 13, CurrentMaterial.GetValues());
	glUniform1i(CurrentShader->GetLocation(Location::L_NORMAL_MAP), CurrentMaterial.NormalMap.Index);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Indices.size());
}

void Mesh::Update(Mat4D MVP)
{
	if (app.GetInspector()->Selected && gameObject && app.GetInspector()->Selected == gameObject)
	{
	}
	else
	{
		glDepthRange(0.01, 1.0);

		Draw(MVP);
	}
}

void Mesh::Outline(Mat4D MVP)
{
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	// Render the mesh into the stencil buffer.

	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Draw(MVP);

	// Render the thick wireframe version.

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glLineWidth(4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthRange(0, 0.01);
	auto shader = app.GetResourcesManager()->Get<Shader*>("Source/shaders/NoLightsShader");
	if (shader && this->CurrentShader != shader) {
		auto tmp = this->CurrentShader;
		this->CurrentShader = shader;
		Draw(MVP, true);
		this->CurrentShader = tmp;
	}
	else
		Draw(MVP, true);

	glDisable(GL_STENCIL_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, app.camera.input.Wireframe ? GL_LINE : GL_FILL);
	glLineWidth(1);
}

void Mesh::SetShader(Shader* shader)
{
	this->CurrentShader = shader;
}

#pragma region ImGui

static std::string lastMat;

static const char* sShaders[]{ "Default", "RGB", "Normal", "NoLights", "NormalMapping" };
static int CurrScreenShaderIndex = -1;
void Mesh::ShowComponent()
{
	char triangleSize[64];
	snprintf(triangleSize, 64, "%s , Triangles : %d", this->R_Name.c_str(), (int)this->Indices.size());
	ImGui::Text(triangleSize);
	if (ImGui::Button("Change Mesh"))
	{
		ImGui::OpenPopup("Meshs");
	}
	MeshPopup(this);
	ImGui::TextColored(ImVec4(0.8f, 0.f, 0.8f, 1.f), "Material ");
	if (ImGui::TreeNodeEx(CurrentMaterial.Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient", &CurrentMaterial.Ambient[0]);
		ImGui::ColorEdit4("Diffuse", &CurrentMaterial.Diffuse[0]);
		ImGui::ColorEdit4("Specular", &CurrentMaterial.Specular[0]);
		ImGui::DragFloat("Shininess", &CurrentMaterial.Shininess);
		CurrentMaterial.Dirty = true;
		ImGui::TreePop();
	}

	if (ImGui::Button("Set Texture"))
	{
		ImGui::OpenPopup("Textures");
	}
	if (auto tex = Mesh::TexturePopup("Textures"))
		CurrentTexture = tex;

	ImGui::ListBox("Shader", &CurrShaderIndex, sShaders, IM_ARRAYSIZE(sShaders));

	if (LastShaderIndex != CurrShaderIndex) {
		LastShaderIndex = CurrShaderIndex;
		std::string fileName = SHADERPATH + (std::string)sShaders[CurrShaderIndex] + "Shader";
		if (auto sh = app.GetResourcesManager()->Get<Shader*>(fileName))
		{
			CurrentShader = sh;
		}
		else {
			LOG(LogType::L_ERROR, "%s Not found !", fileName.c_str());
		}
	}

	// If Shader is NormalMapping
	if (sShaders[CurrShaderIndex] == "NormalMapping") {
		CurrentMaterial.NormalMap.ShowImGui();
		if (ImGui::Button("Set Normal"))
		{
			ImGui::OpenPopup("Normal");
		}
		if (auto tex = Mesh::TexturePopup("Normal"))
			CurrentMaterial.NormalMap = *tex;
	}
}

static std::vector<Mesh*> Meshs;
void MeshPopup(Mesh* mesh)
{
	if (ImGui::BeginPopupModal("Meshs"))
	{
		// Search Bar
		static ImGuiTextFilter filter;
		filter.Draw();
		std::vector<std::string> lines;
		if (Meshs.empty())
			app.GetResourcesManager()->GetAllMeshs(Meshs);
		for (int i = 0; i < Meshs.size(); i++)
		{
			lines.push_back(Meshs[i]->R_Name);
			if (filter.PassFilter(lines[i].c_str())) {
				if (ImGui::Button(Meshs[i]->R_Name.c_str()))
				{
					mesh->CurrentMaterial = Meshs[i]->CurrentMaterial;
					mesh->CurrentTexture = Meshs[i]->CurrentTexture;
					mesh->Indices = Meshs[i]->Indices;
					mesh->Name = Meshs[i]->Name;
					mesh->R_Name = Meshs[i]->R_Name;
					mesh->Vertices = Meshs[i]->Vertices;
					mesh->TextureUVs = Meshs[i]->TextureUVs;
					mesh->Normals = Meshs[i]->Normals;
					mesh->Positions = Meshs[i]->Positions;
					mesh->CurrentShader = Meshs[i]->CurrentShader;
					mesh->SetLoaded(false);
					mesh->Start();
					ImGui::CloseCurrentPopup();
				}
			}
		}

		if (ImGui::Button("Close") || ImGui::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

Mesh Mesh::GlobalMesh(std::vector<bool>& change)
{
	Mesh result;
	char triangleSize[32];
	snprintf(triangleSize, 32, ", Triangles : %d", (int)this->Indices.size());
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Material
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 255));
		ImGui::BeginChild("##Child1", ImVec2(0, 150), true, ImGuiWindowFlags_None);
		bool tmp = change[0];
		ImGui::Checkbox("##00", &tmp);
		ImGui::SameLine();
		change[0] = tmp;
		ImGui::TextColored(ImVec4(0.8f, 0.f, 0.8f, 1.f), "Material ");
		ImGui::BeginDisabled(!change[0]);
		if (ImGui::TreeNodeEx(CurrentMaterial.Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit4("Ambient", &CurrentMaterial.Ambient[0]);
			ImGui::ColorEdit4("Diffuse", &CurrentMaterial.Diffuse[0]);
			ImGui::ColorEdit4("Specular", &CurrentMaterial.Specular[0]);
			ImGui::DragFloat("Shininess", &CurrentMaterial.Shininess);
			CurrentMaterial.Dirty = true;
			ImGui::TreePop();
		}
		ImGui::EndDisabled();
		ImGui::PopStyleColor();
		ImGui::EndChild();

		// Texture
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 255));
		ImGui::BeginChild("##Child2", ImVec2(0, 150), true, ImGuiWindowFlags_None);
		tmp = change[1];
		ImGui::Checkbox("##01", &tmp);
		change[1] = tmp;
		ImGui::BeginDisabled(!change[1]);

		if (ImGui::Button("Set Texture"))
		{
			ImGui::OpenPopup("Textures");
		}
		if (auto tex = Mesh::TexturePopup("Textures"))
			CurrentTexture = tex;
		ImGui::EndDisabled();
		ImGui::PopStyleColor();
		ImGui::EndChild();

		// Shader
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 255));
		ImGui::BeginChild("Child3", ImVec2(0, 240), true, ImGuiWindowFlags_None);
		tmp = change[2];
		ImGui::Checkbox("##02", &tmp);
		change[2] = tmp;
		ImGui::BeginDisabled(!change[2]);

		ImGui::ListBox("Shader", &CurrShaderIndex, sShaders, IM_ARRAYSIZE(sShaders));

		if (LastShaderIndex != CurrShaderIndex) {
			LastShaderIndex = CurrShaderIndex;
			std::string fileName = SHADERPATH + (std::string)sShaders[CurrShaderIndex] + "Shader";
			if (auto sh = app.GetResourcesManager()->Get<Shader*>(fileName))
			{
				CurrentShader = sh;
			}
			else {
				LOG(LogType::L_ERROR, "%s Not found !", fileName.c_str());
			}
		}

		// If Shader is NormalMapping
		if (sShaders[CurrShaderIndex] == "NormalMapping") {
			CurrentMaterial.NormalMap.ShowImGui();
			if (ImGui::Button("Set Normal"))
			{
				ImGui::OpenPopup("Normal");
			}
			if (auto tex = Mesh::TexturePopup("Normal"))
				CurrentMaterial.NormalMap = *tex;
		}

		ImGui::EndDisabled();
		ImGui::PopStyleColor();
		ImGui::EndChild();

	}
	return result;
}

Texture* Mesh::TexturePopup(std::string WindowName)
{
	Texture* texture = nullptr;
	int i = 0;
	bool open = true;
	if (ImGui::BeginPopupModal(WindowName.c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		for (auto res : app.GetResourcesManager()->Resources)
		{
			if (auto tex = dynamic_cast<Texture*>(res.second))
			{
				i++;
				ImGui::BeginGroup();
				if (ImGui::ImageButton((ImTextureID)(uint64_t)tex->TextureData, ImVec2 { 105, 105 }))
				{
					texture = tex;
					ImGui::CloseCurrentPopup();
				}
				std::string name = tex->Name;
				if (name.size() >= 16)
					name.erase(name.begin(), name.begin() + 16);
				ImGui::Text(name.substr(0, 15).c_str());
				ImGui::EndGroup();
				if (i % 12 == 0)
					ImGui::NewLine();
				else
					ImGui::SameLine();
			}
		}
		ImGui::NewLine();
		if (ImGui::Button("Close") || ImGui::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return texture;
}
#pragma endregion
#pragma region Load/Save

void Mesh::Save(std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Mesh Name\" : \"%s\",\n", R_Name.c_str());
	line += space + "\"Material\":{\n";
	CurrentMaterial.Save(space + '\t', line);
	line += space + "},\n";
	line += space + "\"Texture\":{\n";
	if (CurrentTexture)
		CurrentTexture->Save(space + '\t', line);
	line += space + "},\n";
	line += string_format(space + "\"Shader\" : \"%s\"\n", CurrentShader->Name.c_str());
#else
	line += string_format(space + "Mesh Name : %s\n", R_Name.c_str());
	CurrentMaterial.Save(space, line);
	if (CurrentTexture)
		CurrentTexture->Save(space, line);
	line += string_format(space + "Shader Name : %s\n", CurrentShader->Name.c_str());
#endif
}

Mesh* Mesh::Load(const char* data, uint32_t& pos)
{
	Mesh* mesh = new Mesh();
	while (data[pos] != 'N' && data[pos + 1] != 'a' || data[pos] != 'C' && data[pos + 1] != 'o')
	{
		if (data[pos] == 'M' && data[pos + 1] == 'e')
		{
			std::string Name = Loader::GetString(data, pos, 12);
#if MULTITHREAD
			auto m = app.GetResourcesManager()->Get<Mesh*>(Name);
			if (m != nullptr && m->IsLoaded()) {
				mesh->Copy(m);
			}
			else
			{
				app.WaitingList[this->gameObject] = Name;
			}
#else
			Mesh* tmp = app.GetResourcesManager()->Get<Mesh*>(Name);
			mesh->Copy(tmp);

#endif
		}
		else if (data[pos] == 'M' && data[pos + 1] == 'a')
		{
			mesh->CurrentMaterial = Material::Load(data, pos);
		}
		else if (data[pos] == 'T' && data[pos + 1] == 'e')
		{
			mesh->CurrentTexture = Texture::Load(data, pos);

		}
		else if (data[pos] == 'S' && data[pos + 1] == 'h')
		{
			auto shaderValue = Loader::GetString(data, pos, 14);
			mesh->SetShader(app.GetResourcesManager()->Get<Shader*>(shaderValue));
			break;
		}
		else
		{
			pos++;
		}
	}
	return mesh;
}

#pragma endregion

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Mesh, m) {
	py::class_<Mesh, Component>(m, "Mesh")
		.def_readwrite("CurrentMaterial", &Mesh::CurrentMaterial)
		.def_readwrite("CurrentTexture", &Mesh::CurrentTexture)
		.def("IsLoaded", &Mesh::IsLoaded)
		.def("SetShader", &Mesh::SetShader)
		.def("Copy", &Mesh::Copy);
	m.def("CreateMesh", []() { return new Mesh(); }, pybind11::return_value_policy::reference);
}