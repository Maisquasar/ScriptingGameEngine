#include "../../includes/Physic/Rigidbody.hpp"
#include "../../includes/Core/DataStructure/Node.hpp"
#include "../../includes/Core/App.hpp"


Rigidbody::Rigidbody()
{
	ComponentName = "Rigidbody";
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::Save(std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Gravity\" : %f,\n", this->Gravity);
	line += string_format(space + "\"Mass\" : %f,\n", this->Mass);
	line += string_format(space + "\"Max\" : %f,\n", this->Max);
	line += string_format(space + "\"Friction\" : [ %f, %f, %f ]\n", this->Friction.x, this->Friction.y, this->Friction.y);
#else
	line += string_format(space + "	Gravity : %f\n", this->Gravity);
	line += string_format(space + "	Mass : %f\n", this->Mass);
	line += string_format(space + "	Max : %f\n", this->Max);
	line += string_format(space + "	Friction { %f, %f, %f }\n", this->Friction.x, this->Friction.y, this->Friction.y);
#endif
}

Rigidbody* Rigidbody::Load(const char* data, uint32_t& pos)
{
	Rigidbody* rb = new Rigidbody();
	rb->gameObject = gameObject;
	while (1)
	{
		if (data[pos] == 'G')
		{
			rb->Gravity = Loader::GetFloat(data, pos, 10);
		}
		else if (data[pos] == 'M' && data[pos + 1] == 'a' && data[pos + 2] == 's')
		{
			rb->Mass = Loader::GetFloat(data, pos, 7);
		}
		else if (data[pos] == 'M')
		{
			rb->Max = Loader::GetFloat(data, pos, 6);
		}
		else if (data[pos] == 'F')
		{
			rb->Friction = Loader::GetVec3(data, pos, 11);
			break;
		}
		else {
			pos++;
		}
	}
	rb->Initialize();
	return rb;
}

void Rigidbody::Initialize()
{
	if (auto ps = gameObject->GetComponent<PlayerScript>()) ps->SetRigidbody(this);
	if (auto col = gameObject->GetComponent<Collider>()) col->SetRigidbody(this);
}

void Rigidbody::GameUpdate()
{
	Script->ExecuteFunction("GameUpdate", this, ImGui::GetIO().DeltaTime);
}

void Rigidbody::ApplyGravity(float dt)
{
	Vec3D force = Vec3D(0);
	force += Vec3D(0, 1, 0) * Mass * Gravity; // apply a force
	Velocity += force / Mass * dt;
}

void Rigidbody::Contact(Vec3D fri)
{
	for (int i = 0; i < 3; i++) {
		if (Velocity[i] < 0.f)
			Velocity[i] -= (70 * Velocity[i] / 100) * fri[i] * ImGui::GetIO().DeltaTime;
		else if (Velocity[i] > 0)
			Velocity[i] -= (70 * Velocity[i] / 100) * fri[i] * ImGui::GetIO().DeltaTime;
	}
}

void Rigidbody::AddForce(Vec3D force)
{
	force += force * Mass * 100; // apply a force
	Velocity += force / Mass * ImGui::GetIO().DeltaTime;
}

void Rigidbody::ShowComponent()
{
	ImGui::InputFloat("Gravity", &Gravity);
	ImGui::InputFloat("Mass", &Mass);
	ImGui::DragFloat("Velocity Max", &Max, 0.01f, -100, 100);
	ImGui::DragFloat3("Friction", &Friction.x, 0.01f);


	ImGui::BeginDisabled();
	ImGui::Text("Velocity : %f %f %f", Velocity.x, Velocity.y, Velocity.z);
	ImGui::EndDisabled();
}

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Rigidbody, m) {
	py::class_<Rigidbody, Component>(m, "Rigidbody")
		.def_readwrite("Gravity", &Rigidbody::Gravity)
		.def_readwrite("Mass", &Rigidbody::Mass)
		.def_readwrite("Max", &Rigidbody::Max)
		.def_readwrite("Friction", &Rigidbody::Friction)
		.def_readwrite("Velocity", &Rigidbody::Velocity)
		.def_readwrite("enableGravity", &Rigidbody::enableGravity)
		.def_readwrite("Grounded", &Rigidbody::Grounded)
		.def_readwrite("gameObject", &Rigidbody::gameObject, pybind11::return_value_policy::reference)
		.def("AddForce", &Rigidbody::AddForce)
		.def("Initialize", &Rigidbody::Initialize);
	m.def("CreateRigidbody", []() { return new Rigidbody(); }, pybind11::return_value_policy::reference);

}