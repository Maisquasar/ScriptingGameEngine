#include "../Source/includes/Resources/ResourcesManager.hpp"
#include "../Source/includes/Core/App.hpp"

ResourcesManager::ResourcesManager()
{
	threadManager.Start();
}

ResourcesManager::~ResourcesManager()
{
	for (auto&& i : Resources)
	{
		delete i.second;
	}
	threadManager.DeleteThreads();
}

#pragma region Create

template <>
TxtFile* ResourcesManager::Create<TxtFile*>(const std::string filename)
{
	TxtFile* tmp = new TxtFile(filename.c_str());
	tmp->R_Name = filename;
	Resources[filename] = tmp;
	return tmp;
}

template <>
Shader* ResourcesManager::Create<Shader*>(const std::string filename)
{
	Shader* tmp = new Shader();
	tmp->R_Name = filename;
	if (tmp->R_Name.size() > 28)
		tmp->R_Name.erase(tmp->R_Name.begin(), tmp->R_Name.begin() + 28);
	if (tmp->SetShader(filename) && tmp->SetFragmentShader(filename))
		tmp->Loaded = true;
	Resources[filename] = tmp;
	return tmp;
}


template <>
Model3D* ResourcesManager::Create<Model3D*>(const std::string filename)
{
	Resources[filename] = new Model3D();
#if MULTITHREAD
	threadManager.QueueJob(&ResourcesManager::MultithreadModels, this, filename);
#else
	MultithreadModels(filename);
#endif
	return nullptr;
}

void ResourcesManager::MultithreadModels(const std::string filename)
{
	Model3D* tmp = new Model3D();
	tmp->Name = filename;
	tmp->LoadModel(filename, *this);
	Resources[filename] = tmp;
}
template <>
Texture* ResourcesManager::Create<Texture*>(const std::string filename)
{
	Resources[filename] = new Texture(TexId);
#if MULTITHREAD
	threadManager.QueueJob(&ResourcesManager::MultithreadTexture, this, filename);
#else
	MultithreadTexture(filename);
#endif
	TexId++;
	return nullptr;
}

void ResourcesManager::MultithreadTexture(const std::string filename)
{
	Texture* tmp = dynamic_cast<Texture*>(Resources[filename]);
	tmp->LoadTexture(filename);
#if MULTITHREAD
#else
	tmp->Initialize();
#endif
}

template <>
Mesh* ResourcesManager::Create<Mesh*>(const std::string filename)
{
	Mesh* tmp = new Mesh();
	tmp->SetShader(dynamic_cast<Shader*>(Resources["Source/shaders/DefaultShader"]));
	tmp->R_Name = filename;
	Resources[filename] = tmp;
	return tmp;
}

void ResourcesManager::CreateMesh(const Mesh mesh, const std::string filename)
{
	Mesh* tmp = new Mesh();
	*tmp = mesh;
	if (!threadManager._terminate)
		tmp->SetShader(dynamic_cast<Shader*>(Resources["Source/shaders/DefaultShader"]));
	tmp->R_Name = filename;
	Resources[filename] = tmp;
}
#pragma endregion
#pragma region Getters
template <>
TxtFile* ResourcesManager::Get<TxtFile*>(const std::string filename)
{
	for (auto& i : Resources)
	{
		TxtFile* file = dynamic_cast<TxtFile*>(i.second);
		if (file != nullptr && i.first == filename)
		{
			return file;
		}
	}
	return nullptr;
}


template <>
Model3D ResourcesManager::Get<Model3D>(const std::string filename)
{
	for (auto& i : Resources)
	{
		Model3D* model = dynamic_cast<Model3D*>(i.second);
		if (model != nullptr && i.first == filename)
		{
			return *model;
		}
	}
	return Model3D();
}

template <>
Model3D* ResourcesManager::Get<Model3D*>(const std::string filename)
{
	for (auto& i : Resources)
	{
		if (i.first == filename && dynamic_cast<Model3D*>(i.second))
		{
			return (Model3D*)i.second;
		}
	}
	return nullptr;
}

template <>
Shader* ResourcesManager::Get<Shader*>(const std::string filename)
{
	for (auto& i : Resources)
	{
		if (!threadManager._terminate)
		if (i.first == filename && dynamic_cast<Shader*>(i.second))
		{
			return (Shader*)i.second;
		}
	}
	return nullptr;
}

template <>
Texture ResourcesManager::Get<Texture>(const std::string filename)
{
	for (auto& i : Resources)
	{
		Texture* tex = dynamic_cast<Texture*>(i.second);
		if (tex != nullptr && i.first == filename)
		{
			return *tex;
		}
	}
	return Texture();
}

template <>
Texture* ResourcesManager::Get<Texture*>(const std::string filename)
{
	for (auto& i : Resources)
	{
		if (i.first == filename && dynamic_cast<Texture*>(i.second))
		{
			return (Texture*)i.second;
		}
	}
	return nullptr;
}

template <>
Mesh ResourcesManager::Get<Mesh>(const std::string filename)
{
	for (auto& i : Resources)
	{
		Mesh* mesh = dynamic_cast<Mesh*>(i.second);
		if (mesh != nullptr && i.first == filename)
		{
			return *mesh;
		}
	}
	return Mesh();
}

template <>
Mesh* ResourcesManager::Get<Mesh*>(const std::string filename)
{
	for (auto& i : Resources)
	{
		if (i.first == filename && dynamic_cast<Mesh*>(i.second))
		{
			return (Mesh*)i.second;
		}
	}
	return nullptr;
}


#pragma endregion

void ResourcesManager::Delete(const std::string filename)
{
	if (Resources[filename] == nullptr)
	{
		Debug_Log(LogType::L_ERROR, "%s not found !");
		return;
	}
	delete Resources[filename];
	Resources.erase(filename);
}

void ResourcesManager::GetAllModels(std::vector<Model3D>& models)
{
	for (auto& i : Resources)
	{
		if (auto model = dynamic_cast<Model3D*>(i.second))
			if (i.second->IsLoaded())
				models.push_back(*model);
	}
}

void ResourcesManager::GetAllMeshs(std::vector<Mesh*>& meshs)
{
	for (auto&& i : Resources)
	{
		if (auto t = dynamic_cast<Mesh*>(i.second))
			meshs.push_back(t);
	}
}



namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Resource, m) {
	py::class_<ResourcesManager>(m, "ResourcesManager")
		.def("GetMesh", &ResourcesManager::Get<Mesh*>, pybind11::return_value_policy::reference)
		.def("GetTexture", &ResourcesManager::Get<Texture*>, pybind11::return_value_policy::reference)
		.def("GetShader", &ResourcesManager::Get<Shader*>, pybind11::return_value_policy::reference);
}