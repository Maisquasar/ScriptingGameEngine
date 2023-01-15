#include "../Source/includes/LowRenderer/LightManager.hpp"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
	for (auto light : DirLights)
		delete light;
	for (auto light : PointLights)
		delete light;
	for (auto light : SpotLights)
		delete light;
}

#pragma region AddLightComponent
template <typename T>bool LightManager::AddLightComponent(Node* node) {}
template <>
bool LightManager::AddLightComponent<DirectionalLight>(Node* node)
{
	if (CanAddLight<DirectionalLight>(node))
	{
		DirectionalLight* dir = new DirectionalLight(DIFFUSE(0.2f, 0.2f, 0.2f, 1.f), AMBIENT(1.f, 1.f, 1.f, 1.f), SPECULAR(0.2f, 0.2f, 0.2f, 1.f), GetFreeIndex<DirectionalLight>());
		dir->Dirty = true;
		node->transform.SetDirty(true);
		node->AddComponent(dir);
		DirLights.push_back(dir);
		return true;
	}
	return false;
}
template <>bool LightManager::AddLightComponent<PointLight>(Node* node)
{
	if (CanAddLight<PointLight>(node))
	{
		PointLight* pt = new PointLight(DIFFUSE(0.2f, 0.2f, 0.2f, 1.f), AMBIENT(0.2f, 0.2f, 0.2f, 1.f), SPECULAR(0.2f, 0.2f, 0.2f, 1.f), GetFreeIndex<PointLight>());
		pt->Dirty = true;
		node->transform.SetDirty(true);
		node->AddComponent(pt);
		PointLights.push_back(pt);
		return true;
	}
	return false;
}
template <>bool LightManager::AddLightComponent<SpotLight>(Node* node)
{
	if (CanAddLight<SpotLight>(node))
	{
		SpotLight* spt = new SpotLight(DIFFUSE(0.2f, 0.2f, 0.2f, 1.f), AMBIENT(0.2f, 0.2f, 0.2f, 1.f), SPECULAR(0.2f, 0.2f, 0.2f, 1.f), GetFreeIndex<SpotLight>());
		spt->Dirty = true;
		node->transform.SetDirty(true);
		node->AddComponent(spt);
		SpotLights.push_back(spt);
		return true;
	}
	return false;
}
#pragma endregion

#pragma region CanAddLight
template <typename T>bool LightManager::CanAddLight(Node* node) { return false; }

template<>
bool LightManager::CanAddLight<DirectionalLight>(Node* node)
{
	return (!node->AlreadyHad<DirectionalLight>() && !node->AlreadyHad<PointLight>() && !node->AlreadyHad<SpotLight>() && DirLights.size() < LIGHT_SIZE);
}
template<>
bool LightManager::CanAddLight<PointLight>(Node* node)
{
	return (!node->AlreadyHad<DirectionalLight>() && !node->AlreadyHad<PointLight>() && !node->AlreadyHad<SpotLight>() && PointLights.size() < LIGHT_SIZE);
}
template<>
bool LightManager::CanAddLight<SpotLight>(Node* node)
{
	return (!node->AlreadyHad<DirectionalLight>() && !node->AlreadyHad<PointLight>() && !node->AlreadyHad<SpotLight>() && SpotLights.size() < LIGHT_SIZE);
}
#pragma endregion

#pragma region GetFreeIndex
template <typename T>
int LightManager::GetFreeIndex() {}
template <>
int LightManager::GetFreeIndex<DirectionalLight>()
{
	std::vector<int> index;
	for (auto dir : DirLights)
	{
		index.push_back(dir->Index);
	}
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		if (std::find(index.begin(), index.end(), i) == index.end())
			return i;
	}
	return -1;
}
template <>
int LightManager::GetFreeIndex<PointLight>()
{
	std::list<int> index;
	for (auto pt : PointLights)
	{
		index.push_back(pt->Index);
	}
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		if (std::find(index.begin(), index.end(), i) == index.end())
			return i;
	}
	return -1;
}
template <>
int LightManager::GetFreeIndex<SpotLight>()
{
	std::list<int> index;
	for (auto spt : SpotLights)
	{
		index.push_back(spt->Index);
	}
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		if (std::find(index.begin(), index.end(), i) == index.end())
			return i;
	}
	return -1;
}
#pragma endregion

#pragma region CanBeAdd
template <> bool LightManager::CanBeAdd<DirectionalLight>(Node* node)
{
	if (node->AlreadyHad<DirectionalLight>() || DirLights.size() >= LIGHT_SIZE)
		return false;
	else
		return true;
}
template <>	bool LightManager::CanBeAdd<PointLight>(Node* node)
{
	if (node->AlreadyHad<PointLight>() || PointLights.size() >= LIGHT_SIZE)
		return false;
	else
		return true;
}
template <>	bool LightManager::CanBeAdd<SpotLight>(Node* node)
{
	if (node->AlreadyHad<SpotLight>() || SpotLights.size() >= LIGHT_SIZE)
		return false;
	else
		return true;
}
#pragma endregion

bool LightManager::Add(Node* node)
{
	if (node->IsComponentInside<DirectionalLight>() && DirLights.size() >= LIGHT_SIZE)
		return false;
	if (node->IsComponentInside<SpotLight>() && SpotLights.size() >= LIGHT_SIZE)
		return false;
	if (node->IsComponentInside<PointLight>() && PointLights.size() >= LIGHT_SIZE)
		return false;
	// Add Light To lightManager lists.
	for (auto comp : node->Components)
	{
		if (auto light = dynamic_cast<DirectionalLight*>(comp)) {
			light->Index = GetFreeIndex<DirectionalLight>();
			DirLights.push_back(light);
		}
		if (auto light = dynamic_cast<PointLight*>(comp)) {
			light->Index = GetFreeIndex<PointLight>();
			PointLights.push_back(light);
		}
		if (auto light = dynamic_cast<SpotLight*>(comp)) {
			light->Index = GetFreeIndex<SpotLight>();
			SpotLights.push_back(light);
		}
	}
	// Do it for all child.
	for (auto child : node->children)
	{
		Add(child);
	}
	return true;
}

Node* LightManager::CopyNode(Node* from)
{
	Node* copy = CopyComponents(from);
	copy->children.clear();
	copy->SetGameState(from->GetGameStatePointer());

	for (int i = 0; i < from->children.size(); i++)
	{
		copy->addChild(CopyNode(from->children[i]));
	}
	return copy;
}


Node* LightManager::CopyComponents(Node* from)
{
	Node* copy = new Node();
	*copy = *from;
	copy->Name = from->Name;
	copy->Components.clear();
	for (auto comp : from->Components)
	{
		Component* c = nullptr;
		if (auto mesh = dynamic_cast<Mesh*>(comp)) {
			auto m = new Mesh;
			m->Copy(mesh);
			m->R_Name = mesh->R_Name;
			m->CurrentTexture = mesh->CurrentTexture;
			copy->AddComponent(m);
		}
		else if (auto light = dynamic_cast<DirectionalLight*>(comp)) {
			auto tmp = new DirectionalLight(*light);
			tmp->Index = GetFreeIndex<DirectionalLight>();
			c = tmp;
		}
		else if (auto light = dynamic_cast<SpotLight*>(comp)) {
			auto tmp = new SpotLight(*light);
			tmp->Index = GetFreeIndex<SpotLight>();
			c = tmp;
		}
		else if (auto light = dynamic_cast<PointLight*>(comp)) {
			auto tmp = new PointLight(*light);
			tmp->Index = GetFreeIndex<PointLight>();
			c = tmp;
		}
		//copy->AddComponent(c);
	}
	return copy;
}

void LightManager::Update(Shader* ShaderProgram)
{
	glUseProgram(ShaderProgram->Program);
	// Set and Resize list of lights.
	std::vector<float> dir;
	std::vector<float> pt;
	std::vector<float> spt;
	dir.resize(8 * 21);
	pt.resize(8 * 21);
	spt.resize(8 * 26);

	for (auto i : DirLights) {
		i->Update(ShaderProgram->Program);
		// Remove light from list if index == -1
		if (i->Index == -1) {
			DirLights.remove(i);
			break;
		}
		// Else Get Value to put into list.
		else {
			auto tmp = i->GetValue();
			for (int j = 0; j < 21; j++) {
				size_t id = ((size_t)21 * (size_t)i->Index) + (size_t)j;
				dir[id] = tmp[j];
			}
		}
	}

	for (auto i : PointLights) {
		i->Update(ShaderProgram->Program);
		// Remove light from list if index == -1
		if (i->Index == -1) {
			PointLights.remove(i);
			break;
		}
		// Else Get Value to put into list.
		else {
			auto tmp = i->GetValue();
			for (int j = 0; j < 21; j++) {
				size_t id = (21 * (size_t)i->Index) + (size_t)j;
				pt[id] = tmp[j];
			}
		}
	}
	for (auto i : SpotLights) {
		i->Update(ShaderProgram->Program);
		// Remove light from list if index == -1
		if (i->Index == -1) {
			SpotLights.remove(i);
			break;
		}
		// Else Get Value to put into list.
		else {
			auto tmp = i->GetValue();
			for (int j = 0; j < 26; j++) {
				size_t id = ((size_t)26 * (size_t)i->Index) + (size_t)j;
				spt[id] = tmp[j];
			}
		}
	}
	// Send it to shader.
	glUniform1fv(ShaderProgram->GetLocation(Location::L_DIRECTIONAL), 8 * 21, &dir[0]);
	glUniform1fv(ShaderProgram->GetLocation(Location::L_POINT), 8 * 21, &pt[0]);
	glUniform1fv(ShaderProgram->GetLocation(Location::L_SPOT), 8 * 26, &spt[0]);
}