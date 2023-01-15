#include "../Source/includes/Core/DataStructure/Node.hpp"
#include <pybind11/stl.h>
#include "../Source/includes/Core/App.hpp"

Node::Node()
{
	SetGameState(&app.State);
	SetPause(app.GetPause());
}

void Node::Delete()
{
	for (int i = (int)this->children.size() - 1; i >= 0; i--)
	{
		this->children[i]->Delete();
	}
	// If Component is Light
	auto light = GetComponentsInChildrens<Light*>();
	if (!light.empty())
	{
		for (auto lt : light)
		{
			lt->Index = -1;
			lt->enable = false;
			lt->Dirty = true;
		}
	}
	light.clear();

	auto col = GetComponentsInChildrens<Collider*>();
	if (!col.empty())
	{
		for (auto cl : col)
		{
			cl->Deleted = true;
		}
	}
	col.clear();
	// Remove from parent
	if (!parent)
		return;
	for (int j = 0; j < this->parent->children.size(); j++)
	{
		if (this->parent->children[j] == this)
		{
			this->parent->children.erase(this->parent->children.begin() + j);
			break;
		}
	}
}

void Node::addChild(Node* node)
{
	children.push_back(node);
	children.back()->parent = this;
}

void Node::AddComponent(Component* comp)
{
	Components.push_back(comp);
	comp->Script = app.ScriptManager.GetScript(comp->ComponentName);
	comp->gameObject = this;
	comp->Start();
}

void Node::SetWorldPosition(Vec3D pos)
{
	if (parent)
		// Set local position in function of parent position.
		transform.SetLocalPosition(pos - parent->transform.GetWorldPosition());
	else
		transform.SetLocalPosition(pos);
}

void Node::SetWorldRotation(Vec3D rot)
{
	if (parent)
		// Set local rotation in function of parent rotation.
		transform.SetLocalRotation(rot - parent->transform.GetWorldRotation());
	else
		transform.SetLocalRotation(rot);
}

void Node::SetWorldScale(Vec3D sca)
{
	if (parent)
		// Set local scale in function of parent scale.
		transform.SetLocalScale(Vec3D(sca.x / parent->transform.GetWorldScale().x, sca.y / parent->transform.GetWorldScale().y, sca.z / parent->transform.GetWorldScale().z));
	else
		transform.SetLocalScale(sca);
}

void Node::updateSelfAndChild()
{
	if (!transform.IsDirty())
		return;
	forceUpdateSelfAndChild();
}

void Node::forceUpdateSelfAndChild()
{
	// Parent-Child relative Transform
	if (parent)
		transform.ComputeModelMatrix(parent->transform.GetModelMatrix());
	else
		transform.ComputeModelMatrix();

	// Set Rotation between 0 - 360.
	transform.SetLocalRotation(Vec3D(math::Mod(transform.GetLocalRotation().x, 360), math::Mod(transform.GetLocalRotation().y, 360), math::Mod(transform.GetLocalRotation().z, 360)));
	for (auto comp : Components)
	{
		if (auto light = dynamic_cast<Light*>(comp)) {
			light->Position = transform.GetWorldPosition();
			light->Dirty = true;
		}
	}
	transform.SetDirty(false);

	//Update Transform of Childs.
	for (auto&& child : children)
	{
		child->forceUpdateSelfAndChild();
	}
}

void Node::UpdateComponents(Mat4D vp)
{
	updateSelfAndChild();
	Rigidbody* lastUpdate = nullptr;
	for (int i = 0; i < Components.size(); i++)
	{
		if (!Components[i]->windowOpen) {
			Components.erase(Components.begin() + i);
			break;
		}
		if (!Components[i]->enable)
			continue;
		//Draw Mesh.
		if (auto mesh = dynamic_cast<Mesh*>(Components[i]))
		{
			Mat4D MVP = vp * this->transform.GetModelMatrix();
			mesh->Update(MVP);
		}
		else if (auto collider = dynamic_cast<Collider*>(Components[i]))
		{
			collider->Update(vp);
		}
		else
		{
			if (GetGameState() == GameState::GS_PLAY)
				Components[i]->GameUpdate();
			else
				Components[i]->EditorUpdate();
			Components[i]->Update();
		}
	}
	for (auto child : children)
		child->UpdateComponents(vp);
}

std::list<Node*> Node::GetAllChildrens()
{
	std::list<Node*> node;
	for (auto&& child : children)
	{
		auto cc = child->GetAllChildrens();
		for (auto&& c : cc)
		{
			node.push_back(c);
		}
		node.push_back(child);
	}
	return node;
}

void Node::ShowImGui(Node*& last, int index)
{
	ImGui::PushID(index);
	if (ImGui::TreeNodeEx(Name.c_str(), children.size() == 0 ? ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth : ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
	{
		if (!_lastState) {
			_lastState = true;
			last = this;
		}
		for (auto&& child : children)
		{
			index++;
			child->ShowImGui(last, index);
		}
		ImGui::TreePop();
	}
	else
	{
		if (_lastState) {
			_lastState = false;
			last = this;
		}
	}
	_hover = ImGui::IsItemHovered();
	for (auto&& child : children)
	{
		if (child->_hover)
			_hover = false;
	}
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		last = this;
	_leftClicked = ImGui::IsItemClicked();
	ImGui::PopID();
}

#pragma region Load/Save
void Node::Save(std::ofstream& _file, std::string space)
{
#if JSON
	// Write Into file.
	std::string line;
	line += string_format("\n" + space + "\"Name\" : \"%s\",\n", this->Name.c_str());
	_file.write(line.c_str(), line.size());
	line.clear();
	// Save Transform.
	transform.Save(_file, space);

	line.clear();
	if (Components.size() > 0) {
		line += space + "\"Component\" :\n";
		line += space + "[{\n";
	}
	else
		line += space + "\"Component\" : [],\n";
	_file.write(line.c_str(), line.size());
	line.clear();
	// Save all Components.
	int i = 0;
	for (auto comp : Components)
	{
		i++;
		line += space + '\t' + string_format("\"Type\" : \"%s\",\n", comp->ComponentName.c_str());
		comp->Save(space + '\t', line);
		if (i == Components.size())
			line += space + "}],\n";
		else {
			line += space + "},\n";
			line += space + "{\n";
		}

		_file.write(line.c_str(), line.size());
		line.clear();
	}
	// Add Space. (formatting only)
	if (children.size() > 0) {
		line += space + "\"Children\":\n";
		line += space + "[{";
		_file.write(line.c_str(), line.size());
	}
	else
	{
		line += space + "\"Children\": []\n";
		_file.write(line.c_str(), line.size());
	}
	line.clear();
	// Save all Childrens.
	i = 0;
	for (auto child : children)
	{
		i++;
		child->Save(_file, space + '\t');
		if (i == children.size())
			line += space + "}]\n";
		else
		{
			line += space + "},\n";
			line += space + "{\n";
		}
		_file.write(line.c_str(), line.size());
		line.clear();
	}
#else
	// Write Into file.
	std::string line;
	line += string_format("\n" + space + "Name : %s\n", this->Name.c_str());
	if (parent != nullptr) {
		line += string_format(space + "Parent : %s\n", parent->Name.c_str());
}
	line += string_format(space + "Number of Children : %d\n", children.size());
	_file.write(line.c_str(), line.size());
	line.clear();
	// Save Transform.
	transform.Save(_file, space);

	// Save all Components.
	for (auto comp : Components)
	{
		std::string line;
		line += space + "Component :\n";
		line += space + string_format("Type : %s\n", comp->ComponentName.c_str());
		comp->Save(space, line);
		_file.write(line.c_str(), line.size());
	}
	// Add Space. (formatting only)
	if (children.size() > 0)
		space += "	";
	// Save all Childrens.
	for (auto child : children)
	{
		child->Save(_file, space);
	}
#endif // JSON
}

Node* Node::Load(const char* data, uint32_t& pos, std::unordered_map<std::string, IResource*> res)
{
	Node* node = new Node();
	int childSize = 0;
	bool first = true;

	while (data[pos] != '\0')
	{
		if (data[pos] == 'N' && data[pos + 1] == 'a')
		{
			if (first) {
				node->Name = Loader::GetString(data, pos, 7);
				first = false;
			}
			// If not the first time then new node, so break.
			else
				break;
		}
		// Get Child Size.
		else if (data[pos] == 'N' && data[pos + 1] == 'u')
		{
			childSize = Loader::GetInt(data, pos, 21);
		}
		// Get Position.
		else if (data[pos] == 'P' && data[pos + 1] == 'o')
		{
			node->transform.SetLocalPosition(Loader::GetVec3(data, pos, 11));
		}
		// Get Rotation.
		else if (data[pos] == 'R' && data[pos + 1] == 'o')
		{
			node->transform.SetLocalRotation(Loader::GetVec3(data, pos, 11));
		}
		// Get Scale.
		else if (data[pos] == 'S' && data[pos + 1] == 'c')
		{
			node->transform.SetLocalScale(Loader::GetVec3(data, pos, 8));
		}
		// Get Model Matrix.
		else if (data[pos] == 'W' && data[pos + 1] == 'o')
		{
			node->transform.ModelMatrix = Loader::GetMat4(data, pos, 17);
		}
		// Get Components.
		else if (data[pos] == 'C' && data[pos + 1] == 'o')
		{
			node->LoadComponent(data, pos, res);
		}
		else if (data[pos] == 'P' && data[pos + 1] == 'a')
		{
			Loader::SkipLine(data, pos);
		}
		else
		{
			pos++;
		}
	}
	// Add Childs.
	for (int i = 0; i < childSize; i++)
	{
		node->addChild(Node::Load(data, pos, res));
	}
	return node;
}

void Node::LoadComponent(const char* data, uint32_t& pos, std::unordered_map<std::string, IResource*> res)
{
	// Get Type of the Components.
	std::string Type;
	while (1)
	{
		if (data[pos] == 'T' && data[pos + 1] == 'y')
		{
			Type = Loader::GetString(data, pos, 7);
			break;
		}
		else
			pos++;
	}
	for (int i = 0; i < CData.Datas.size(); i++)
	{
		if (Type == CData.Datas[i]->ComponentName)
		{
			CData.Datas[i]->gameObject = this;
			auto temp = CData.Datas[i]->Load(data, pos);
			CData.Datas[i]->gameObject = nullptr;
			AddComponent(temp);
			break;
		}
	}
}
// Scene loop.
Node* Node::Loop(const char* data, uint32_t size, std::unordered_map<std::string, IResource*> res)
{
	uint32_t pos = 0;
	return Node::Load(data, pos, res);
}

Node* Node::LoadScene(std::string filename, std::unordered_map<std::string, IResource*> res)
{
	uint32_t size = 0;
	bool sucess = false;
	auto data = Loader::ReadFile(filename.c_str(), size, sucess);
	if (sucess)
	{
		Node* Scene = Node::Loop(data, size, res);
		LOG(LogType::INFO, "Sucessfuly Load Scene : %s", filename.c_str());
		delete[] data;
		return Scene;
	}
	delete[] data;
	return nullptr;
}

#pragma endregion

bool Node::IsHovered()
{
	return _hover;
}

void Node::SetLastState(bool last)
{
	_lastState = last;
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(GameObject, m) {
	py::class_<Node>(m, "Node")
		.def_readwrite("transform", &Node::transform)
		.def_readwrite("children", &Node::children)
		.def_readwrite("Components", &Node::Components)
		.def_readwrite("Name", &Node::Name)
		.def("addChild", &Node::addChild)
		.def("AddComponent", &Node::AddComponent);
	m.def("CreateNode", []() { return new Node(); }, pybind11::return_value_policy::reference);
}