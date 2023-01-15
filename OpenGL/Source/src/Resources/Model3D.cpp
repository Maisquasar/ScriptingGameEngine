#include "../Source/includes/Resources/Model3D.hpp"
#include "../Source/includes/Core/App.hpp"
Model3D::Model3D() {
}
Model3D::~Model3D() {}

void SkipLine(const char* data, int32_t& pos);
Vec3D GetVector3(const char* data, int32_t& pos, int dec);
Vec2D GetVector2(const char* data, int32_t& pos, int dec);
std::vector<Int3D> GetIndex(const char* data, int32_t& pos);
float GetFloat(const char* data, int32_t& pos, int dec);
std::string GetString(const char* data, int32_t& pos, int dec);

void Model3D::LoadModel(std::string filename, ResourcesManager& manager)
{
	Name = filename;
	std::string tmp = MeshPath;
	tmp += filename;
	filename = tmp.c_str();
	uint32_t size;
	bool success = false;
	auto datas = Loader::ReadFile(filename.c_str(), size, success);
	if (datas == nullptr || !success) return;

	ModelLoop(datas, size, manager);

	LOG(LogType::INFO, "Successfully loaded model: %s", filename.c_str());
}

bool Model3D::IsLoaded()
{
	return Loaded;
}

#pragma region ModelLoader

void Model3D::MaterialLoop(const char* data, const int32_t& size, ResourcesManager& manager)
{
	int32_t pos = 0;

	std::string mtlPath;
	std::string M_Name;
	Material* mat = nullptr;

	while (pos < size)
	{
		// Get Material Name.
		if (data[pos] == 'n' && data[pos + 1] == 'e')
		{
			if (mat != nullptr) {
				Materials[M_Name] = *mat;
			}
			mat = new Material();
			mat->Tex = nullptr;
			M_Name = GetString(data, pos, 7);
			mat->Name = M_Name;
			continue;
		}
		// Get Shininess.
		else if (data[pos] == 'N' && data[pos + 1] == 's' && mat != nullptr)
		{
			mat->Shininess = GetFloat(data, pos, 3);
		}
		// Get Ambient.
		else if (data[pos] == 'K' && data[pos + 1] == 'a' && mat != nullptr)
		{
			mat->Ambient = GetVector3(data, pos, 3);
		}
		// Get Diffuse.
		else if (data[pos] == 'K' && data[pos + 1] == 'd' && mat != nullptr)
		{
			mat->Diffuse = GetVector3(data, pos, 3);
		}
		// Get Specular.
		else if (data[pos] == 'K' && data[pos + 1] == 's' && mat != nullptr)
		{
			mat->Specular = GetVector3(data, pos, 3);
		}
		// Get Transparency.
		else if (data[pos] == 'd' && mat != nullptr)
		{
			float transparency = GetFloat(data, pos, 2);
			mat->Ambient.w = transparency;
			mat->Specular.w = transparency;
			mat->Diffuse.w = transparency;
		}
		// Get/Create Texture.
		else if (data[pos] == 'm' && mat != nullptr)
		{
			std::string path = GetString(data, pos, 7);
			if (manager.Get<Texture*>(path.c_str()) != nullptr)
			{
				mat->Tex = manager.Get<Texture*>(path.c_str());
			}
			else
			{
				LOG(LogType::WARNING, "Cannot Find Texture %s !", path.c_str());
				mat->TexName = path;
			}
		}
		// Else Skip.
		else
		{
			SkipLine(data, pos);
		}
	}
	if (mat != nullptr)
		Materials[M_Name] = *mat;

}

void Model3D::ModelLoop(const char* data, const int32_t& size, ResourcesManager& manager)
{
	int32_t pos = 0;

	std::string mtlPath;
	uint32_t Mtlsize;
	bool sucess;
	Int3D lastSize = Int3D(0, 0, 0);
	std::string Mn;
	bool FindMaterial = false;
	bool mtl = false;
	std::vector<Mesh> Meshs;

	while (pos < size)
	{
		if (manager.threadManager._terminate)
			return;
		// Get Mesh Name.
		if (data[pos] == 'o')
		{
			if (Meshs.size() > 0) {
				lastSize = lastSize + Int3D((int)Meshs.back().Positions.size(), (int)Meshs.back().TextureUVs.size(), (int)Meshs.back().Normals.size());
			}
			Meshs.push_back(Mesh());
			Meshs.back().Name = GetString(data, pos, 2);
		}
		//Get MTL file and read it.
		else if (data[pos] == 'm')
		{
			mtlPath = GetString(data, pos, 7);
			auto data = Loader::ReadFile((MeshPath + mtlPath).c_str(), Mtlsize, sucess);
			if (sucess) {
				MaterialLoop(data, Mtlsize, manager);
				mtl = true;
			}
		}
		//Get Material.
		else if (data[pos] == 'u')
		{
			// If Mtl not found : skip.
			if (!mtl)
			{
				SkipLine(data, pos);
				continue;
			}
			Mn = GetString(data, pos, 7);
			// If No Indices found do not create a other mesh.
			if (Meshs.back().Indices.size() != 0) {
				auto tmp = Mesh(Meshs.back());
				tmp.Indices.clear();
				Meshs.push_back(tmp);
			}
			Meshs.back().Name = Mn;

			//New Material, set End position (index position) to precedent Material.
			// Get the Material name.
			// Find the Material.
			auto search = Materials.find(Mn);
			if (search != Materials.end())
			{
				// Set Material
				Meshs.back().CurrentMaterial = Materials[Mn];
				if (Materials[Mn].Tex != nullptr) {
					Meshs.back().CurrentTexture = Materials[Mn].Tex;
				}
				FindMaterial = true;
			}
			else
			{
				FindMaterial = false;
				LOG(LogType::L_ERROR, "Material : %s Can't be found", Mn);
			}
		}
		// Get Position.
		else if (data[pos] == 'v' && data[pos + 1] == ' ')
		{
			Meshs.back().Positions.push_back(GetVector3(data, pos, 2));
		}
		// Get TextureUV.
		else if (data[pos] == 'v' && data[pos + 1] == 't')
		{
			Meshs.back().TextureUVs.push_back(GetVector2(data, pos, 3));
		}
		// Get Normal.
		else if (data[pos] == 'v' && data[pos + 1] == 'n')
		{
			Meshs.back().Normals.push_back(GetVector3(data, pos, 3));
		}
		// Get Indices.
		else if (data[pos] == 'f')
		{
			std::vector<Int3D> tmp = GetIndex(data, pos);
			for (int i = 0; i < tmp.size(); i++)
				Meshs.back().Indices.push_back(tmp[i] - lastSize);
		}
		// Else Skip.
		else
		{
			SkipLine(data, pos);
			continue;
		}
	}
	for (int i = 0; i < Meshs.size(); i++)
	{
#if MULTITHREAD

		// set Mesh In resources Manager.
		Meshs[i].Start();
		Meshs[i].R_Name = this->Name + "::" + Meshs[i].Name;
		manager.CreateMesh(Meshs[i], Meshs[i].R_Name);
		Node* node = new Node();
		node->Name = Meshs[i].Name;

		auto copy = new Mesh(Meshs[i]);
		copy->R_Name = Meshs[i].R_Name;
		node->AddComponent(copy);
		this->addChild(node);
#else
		// set Mesh In resources Manager.
		Meshs[i].R_Name = this->Name + "::" + Meshs[i].Name;
		manager.CreateMesh(Meshs[i], Meshs[i].R_Name);
		Node* node = new Node();
		node->Name = Meshs[i].Name;

		node->AddComponent(new Mesh(Meshs[i]));
		auto mesh = dynamic_cast<Mesh*>(node->Components.back());
		// Set Default Shader and Init.
		mesh->SetShader(dynamic_cast<Shader*>(manager.Resources["Source/shaders/DefaultShader"]));
		// Then add to child.
		this->addChild(node);

#endif // MULTITHREAD
	}
	Loaded = true;
}

void SkipLine(const char* data, int32_t& pos)
{
	while (data[pos] != '\0' && data[pos] != '\n')
	{
		pos++;
	}
	pos++;
}
#pragma endregion

#pragma region Getters

Vec3D GetVector3(const char* data, int32_t& pos, const int dec)
{
	Vec3D position;
	std::string str;
	pos += dec;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	position.x = std::stof(str);
	str.clear();
	pos++;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	position.y = std::stof(str);
	str.clear();
	pos++;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	position.z = std::stof(str);
	str.clear();
	pos++;

	return position;
}

Vec2D GetVector2(const char* data, int32_t& pos, const int dec)
{
	Vec2D texUV;
	std::string str;
	pos += dec;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	texUV.x = std::stof(str);
	str.clear();
	pos++;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	texUV.y = std::stof(str);
	str.clear();
	pos++;

	return texUV;
}

std::vector<Int3D> GetIndex(const char* data, int32_t& pos)
{
	std::vector<Int3D> index;
	std::string str;
	int i = 0;
	pos += 2;
	while (data[pos] != '\0' && data[pos] != '\n') {
		index.push_back(Int3D());
		while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != '/')
		{
			str.push_back(data[pos]);
			pos++;
		}
		index[i].x = std::stoi(str) - 1;
		str.clear();
		pos++;

		while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != '/')
		{
			str.push_back(data[pos]);
			pos++;
		}
		index[i].y = std::stoi(str) - 1;

		str.clear();
		pos++;

		while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != '/')
		{
			str.push_back(data[pos]);
			pos++;
		}
		index[i].z = std::stoi(str) - 1;
		str.clear();
		i++;
		if (data[pos] == ' ')
			pos++;
	}
	return index;
}

float GetFloat(const char* data, int32_t& pos, const int dec)
{
	std::string str;
	pos += dec;
	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ')
	{
		str.push_back(data[pos]);
		pos++;
	}
	return std::stof(str);
}

std::string GetString(const char* data, int32_t& pos, const int dec)
{
	std::string name;
	pos += dec;
	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != '\r')
	{
		name.push_back(data[pos]);
		pos++;
	}
	pos++;
	return name;
}

#pragma endregion