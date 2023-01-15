#include "../Source/includes/Resources/Components/PlayerScript.hpp"
#include "../Source/includes/Core/App.hpp"
PlayerScript::~PlayerScript()
{
}

void PlayerScript::InitKeys()
{
	if (app.PlayerPrefs.GetInteger("Dirty") == 0) {
		Key["Forward"] = ImGuiKey_W;
		Key["Backward"] = ImGuiKey_S;
		Key["Left"] = ImGuiKey_A;
		Key["Right"] = ImGuiKey_D;
		Key["Jump"] = ImGuiKey_Space;
	}
	else
	{
		for (auto binds : KeysName)
		{
			Key[binds] = app.PlayerPrefs.GetInteger(binds);
		}
	}
}

void PlayerScript::Start()
{
	InitKeys();
	this->SetCamera(&app.camera);
	_rb = gameObject->GetComponent<Rigidbody>();
}

void PlayerScript::GameUpdate()
{
	if (app.PlayerPrefs.GetInteger("Dirty") == true)
	{
		app.PlayerPrefs.SetInteger("Dirty", false);
		for (auto binds : KeysName)
		{
			Key[binds] = app.PlayerPrefs.GetInteger(binds);
		}
	}
	if (IsInPause())
		return;
	// Apply Movement.
	Script->ExecuteFunction("GameUpdate", this, ImGui::GetIO().DeltaTime);
}

void PlayerScript::Save(const std::string space, std::string& line)
{
#if JSON
	line += space + string_format("\"Speed\" : %f,\n", Speed);
	line += space + string_format("\"Jump\" : %f\n", JumpForce);
#else
	line += space + string_format("Speed : %f\n", Speed);
	line += space + string_format("Jump : %f\n", JumpForce);
#endif
}

PlayerScript* PlayerScript::Load(const char* data, uint32_t& pos)
{
	PlayerScript* ps = new PlayerScript();
	while (1)
	{
		if (data[pos] == 'S')
		{
			ps->Speed = Loader::GetFloat(data, pos, 8);
		}
		else if (data[pos] == 'J')
		{
			ps->JumpForce = Loader::GetFloat(data, pos, 7);
			break;
		}
		else {
			pos++;
		}
	}
	return ps;
}

void PlayerScript::ShowComponent()
{
	ImGui::DragFloat("Speed", &Speed, 0.01f, 0);
	ImGui::DragFloat("JumpForce", &JumpForce, 0.01f, 0);
}

void PlayerScript::InputUpdate()
{
	this->_input.forward = ImGui::IsKeyDown(Key["Forward"]);
	this->_input.backward = ImGui::IsKeyDown(Key["Backward"]);
	this->_input.right = ImGui::IsKeyDown(Key["Right"]);
	this->_input.left = ImGui::IsKeyDown(Key["Left"]);
	this->_input.jump = ImGui::IsKeyDown(Key["Jump"]);
}

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Player, m) {
	py::class_<PlayerInput>(m, "PlayerInput")
		.def_readwrite("forward", &PlayerInput::forward)
		.def_readwrite("backward", &PlayerInput::backward)
		.def_readwrite("left", &PlayerInput::left)
		.def_readwrite("right", &PlayerInput::right)
		.def_readwrite("jump", &PlayerInput::jump);

	py::class_<PlayerScript, Component>(m, "PlayerScript")
		.def("InputUpdate", &PlayerScript::InputUpdate)
		.def_readwrite("Speed", &PlayerScript::Speed)
		.def_readwrite("JumpForce", &PlayerScript::JumpForce)
		.def_property("_input", &PlayerScript::GetInput, &PlayerScript::SetInput)
		.def_property("_rb", &PlayerScript::GetRigidbody, &PlayerScript::SetRigidbody)
		.def_property("_cam", &PlayerScript::GetCamera, &PlayerScript::SetCamera);
	m.def("CreatePlayerScript", []() { return new PlayerScript(); }, pybind11::return_value_policy::reference);

}