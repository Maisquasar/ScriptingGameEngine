#include "../Source/includes/LowRenderer/Light.hpp"
#include "../Source/includes/Core/App.hpp"

Light::Light() {}

Light::Light(Vec4D dif, Vec4D amb, Vec4D spe)
{
	Diffuse = dif;
	Ambient = amb;
	Specular = spe;
}
Light::~Light() {}
void Light::SetLight(Vec4D dif, Vec4D amb, Vec4D spe)
{
	Diffuse = dif;
	Ambient = amb;
	Specular = spe;
}
PointLight::PointLight()
{
	ComponentName = "Point Light";
}

PointLight::PointLight(int index)
{
	this->Index = index;
	ComponentName = "Point Light";

}
PointLight::PointLight(Vec4D dif, Vec4D amb, Vec4D spe, int index)
{
	Diffuse = dif;
	Ambient = amb;
	Specular = spe;
	this->Index = index;
	ComponentName = "Point Light";
}

void PointLight::Start()
{
	if (this->Index == -1)
		this->Index = app.GetLightManager()->GetFreeIndex<PointLight>();
	app.GetLightManager()->PointLights.push_back(this);
}

void PointLight::Update(GLuint shader)
{
	if (Dirty)
		UpdateValues();
	if (!windowOpen) {
		enable = false;
		Index = -1;
	}

}

void PointLight::SetComponent(float cst, float lin, float qua)
{
	Constant = cst;
	Linear = lin;
	Quadratic = qua;
	Dirty = true;
}
void PointLight::SetComponent(Vec3D parameters)
{
	Constant = parameters.x;
	Linear = parameters.y;
	Quadratic = parameters.z;
	Dirty = true;
}

void PointLight::Save(std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Index\" : %d, \n", this->Index);
	line += string_format(space + "\"Ambient\" : [ %f, %f, %f, %f ],\n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "\"Diffuse\" : [ %f, %f, %f, %f ],\n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "\"Specular\" : [ %f, %f, %f, %f ],\n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "\"Strenght\" : [ %f, %f ],\n", this->AmbientStrenght, this->SpecularStrenght);
	line += string_format(space + "\"Parameters\" : [ %f, %f, %f ]", this->Constant, this->Linear, this->Quadratic);
#else
	line += string_format(space + "	Index : %d \n", this->Index);
	line += string_format(space + "	Ambient { %f, %f, %f, %f } \n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "	Diffuse { %f, %f, %f, %f } \n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "	Specular { %f, %f, %f, %f } \n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "	Strenght { %f, %f } \n", this->AmbientStrenght, this->SpecularStrenght);
	line += string_format(space + "	Parameters { %f, %f, %f }", this->Constant, this->Linear, this->Quadratic);
#endif
}

PointLight* PointLight::Load(const char* data, uint32_t& pos)
{
	PointLight* pt = new PointLight();
	while (1)
	{
		if (data[pos] == 'I')
		{
			pt->Index = Loader::GetInt(data, pos, 8);
		}
		else if (data[pos] == 'A')
		{
			pt->Ambient = Loader::GetVec4(data, pos, 10);
		}
		else if (data[pos] == 'D')
		{
			pt->Diffuse = Loader::GetVec4(data, pos, 10);
		}
		else if (data[pos] == 'S' && data[pos + 1] == 'p')
		{
			pt->Specular = Loader::GetVec4(data, pos, 11);
		}
		else if (data[pos] == 'S' && data[pos + 1] == 't')
		{
			auto tmp = Loader::GetVec2(data, pos, 11);
			pt->AmbientStrenght = tmp.x;
			pt->SpecularStrenght = tmp.y;
		}
		else if (data[pos] == 'P' && data[pos + 1] == 'a')
		{
			pt->SetComponent(Loader::GetVec3(data, pos, 13));
			break;
		}
		else {
			pos++;
		}
	}
	return pt;
}

void PointLight::ShowComponent()
{
	ImGui::ColorEdit3("Diffuse", &Diffuse[0]);
	ImGui::ColorEdit3("Ambient", &Ambient[0]);
	ImGui::ColorEdit3("Specular", &Specular[0]);
	ImGui::DragFloat("Ambient Strenght", &AmbientStrenght, 0.001f, 0, FLT_MAX);
	ImGui::DragFloat("Specular Strenght", &SpecularStrenght, 0.01f, 0, FLT_MAX);
	if (ImGui::BeginTable("Test", 3)) {
		ImGui::TableNextColumn();
		ImGui::Text("Constant");
		ImGui::TableNextColumn();
		ImGui::Text("Quadratic");
		ImGui::TableNextColumn();
		ImGui::Text("Linear");
		ImGui::TableNextColumn();
		ImGui::DragFloat("##1", &Constant, 0.01f, 0, FLT_MAX);
		ImGui::TableNextColumn();
		ImGui::DragFloat("##2", &Quadratic, 0.01f, 0, FLT_MAX);
		ImGui::TableNextColumn();
		ImGui::DragFloat("##3", &Linear, 0.01f, 0, FLT_MAX);
	}
	ImGui::EndTable();
	Dirty = true;
}

float* PointLight::GetValue()
{
	return _values;
}

void PointLight::UpdateValues()
{
	_values[0] = enable;
	_values[1] = Constant;
	_values[2] = Linear;
	_values[3] = Quadratic;
	_values[4] = Position.x;
	_values[5] = Position.y;
	_values[6] = Position.z;
	_values[7] = Diffuse.x;
	_values[8] = Diffuse.y;
	_values[9] = Diffuse.z;
	_values[10] = Diffuse.w;
	_values[11] = Ambient.x;
	_values[12] = Ambient.y;
	_values[13] = Ambient.z;
	_values[14] = Ambient.w;
	_values[15] = Specular.x;
	_values[16] = Specular.y;
	_values[17] = Specular.z;
	_values[18] = Specular.w;
	_values[19] = AmbientStrenght;
	_values[20] = SpecularStrenght;
	Dirty = false;
}

PointLight::~PointLight() {}

DirectionalLight::DirectionalLight()
{
	ComponentName = "Directional Light";
}
DirectionalLight::DirectionalLight(int index)
{
	this->Index = index;
	ComponentName = "Directional Light";
}
DirectionalLight::DirectionalLight(Vec4D dif, Vec4D amb, Vec4D spe, int index)
{
	Diffuse = dif;
	Ambient = amb;
	Specular = spe;
	this->Index = index;
	ComponentName = "Directional Light";
}

void DirectionalLight::Update(GLuint shader)
{
	if (Dirty)
		UpdateValues();
	if (!windowOpen) {
		enable = false;
		this->Index = -1;
	}
}

void DirectionalLight::Start()
{
	if (this->Index == -1)
		this->Index = app.GetLightManager()->GetFreeIndex<DirectionalLight>();
	app.GetLightManager()->DirLights.push_back(this);
}

void DirectionalLight::Save(std::string space, std::string& line)
{

#if JSON
	line += string_format(space + "\"Index\" : %d, \n", this->Index);
	line += string_format(space + "\"Ambient\" : [ %f, %f, %f, %f ],\n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "\"Diffuse\" : [ %f, %f, %f, %f ],\n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "\"Specular\" : [ %f, %f, %f, %f ],\n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "\"Direction\" : [ %f, %f, %f ], \n", this->Direction.x, this->Direction.y, this->Direction.z);
	line += string_format(space + "\"Strenght\" : [ %f, %f ]\n", this->AmbientStrenght, this->SpecularStrenght);
#else
	line += string_format(space + "	Index : %d \n", this->Index);
	line += string_format(space + "	Ambient { %f, %f, %f, %f } \n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "	Diffuse { %f, %f, %f, %f } \n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "	Specular { %f, %f, %f, %f } \n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "	Direction { %f, %f, %f } \n", this->Direction.x, this->Direction.y, this->Direction.z);
	line += string_format(space + "	Strenght { %f, %f } \n", this->AmbientStrenght, this->SpecularStrenght);
#endif
}

DirectionalLight* DirectionalLight::Load(const char* data, uint32_t& pos)
{
	DirectionalLight* dir = new DirectionalLight();
	while (1)
	{
		// Load Index.
		if (data[pos] == 'I')
		{
			dir->Index = Loader::GetInt(data, pos, 8);
		}
		// Load Ambient.
		else if (data[pos] == 'A')
		{
			dir->Ambient = Loader::GetVec4(data, pos, 10);
		}
		// Load Diffuse.
		else if (data[pos] == 'D' && data[pos + 1] == 'i' && data[pos + 2] == 'f')
		{
			dir->Diffuse = Loader::GetVec4(data, pos, 10);
		}
		// Load Specular.
		else if (data[pos] == 'S' && data[pos + 1] == 'p')
		{
			dir->Specular = Loader::GetVec4(data, pos, 11);
		}
		// Load Direction.
		else if (data[pos] == 'D' && data[pos + 1] == 'i' && data[pos + 2] == 'r')
		{
			dir->Direction = Loader::GetVec3(data, pos, 12);
		}
		// Load Strenghts.
		else if (data[pos] == 'S' && data[pos + 1] == 't')
		{
			auto tmp = Loader::GetVec2(data, pos, 11);
			dir->AmbientStrenght = tmp.x;
			dir->SpecularStrenght = tmp.y;
			break;
		}
		else {
			pos++;
		}
	}
	dir->Dirty = true;
	return dir;
}

void DirectionalLight::ShowComponent()
{
	ImGui::ColorEdit3("Diffuse", &Diffuse[0]);
	ImGui::ColorEdit3("Ambient", &Ambient[0]);
	ImGui::ColorEdit3("Specular", &Specular[0]);
	ImGui::DragFloat3("Direction", &Direction[0], 0.01f, -1.f, 1.f);
	ImGui::DragFloat("Ambient Strenght", &AmbientStrenght, 0.001f, 0, FLT_MAX);
	ImGui::DragFloat("Specular Strenght", &SpecularStrenght, 0.01f, 0, FLT_MAX);
	Dirty = true;
}

float* DirectionalLight::GetValue()
{
	return _values;
}

void DirectionalLight::UpdateValues()
{
	_values[0] = enable;
	_values[1] = Direction.x;
	_values[2] = Direction.y;
	_values[3] = Direction.z;
	_values[4] = Position.x;
	_values[5] = Position.y;
	_values[6] = Position.z;
	_values[7] = Diffuse.x;
	_values[8] = Diffuse.y;
	_values[9] = Diffuse.z;
	_values[10] = Diffuse.w;
	_values[11] = Ambient.x;
	_values[12] = Ambient.y;
	_values[13] = Ambient.z;
	_values[14] = Ambient.w;
	_values[15] = Specular.x;
	_values[16] = Specular.y;
	_values[17] = Specular.z;
	_values[18] = Specular.w;
	_values[19] = AmbientStrenght;
	_values[20] = SpecularStrenght;
	Dirty = false;
}
DirectionalLight::~DirectionalLight() {}

SpotLight::SpotLight()
{
	ComponentName = "SpotLight";
}

SpotLight::SpotLight(int index) {
	this->Index = index;
	ComponentName = "SpotLight";
}
SpotLight::SpotLight(Vec4D dif, Vec4D amb, Vec4D spe, int index)
{
	Diffuse = dif;
	Ambient = amb;
	Specular = spe;
	this->Index = index;
	ComponentName = "SpotLight";
}

void SpotLight::Start()
{
	if (this->Index == -1)
		this->Index = app.GetLightManager()->GetFreeIndex<SpotLight>();
	app.GetLightManager()->SpotLights.push_back(this);
}

void SpotLight::Update(GLuint shader)
{
	if (Dirty)
		UpdateValues();
	if (!windowOpen) {
		enable = false;
		Index = -1;
	}
}


float* SpotLight::GetValue()
{
	return _values;
}
void SpotLight::UpdateValues()
{
	_values[0] = enable;
	_values[1] = Direction.x;
	_values[2] = Direction.y;
	_values[3] = Direction.z;
	_values[4] = CutOff;
	_values[5] = OuterCutOff;
	_values[6] = Position.x;
	_values[7] = Position.y;
	_values[8] = Position.z;
	_values[9] = Diffuse.x;
	_values[10] = Diffuse.y;
	_values[11] = Diffuse.z;
	_values[12] = Diffuse.w;
	_values[13] = Ambient.x;
	_values[14] = Ambient.y;
	_values[15] = Ambient.z;
	_values[16] = Ambient.w;
	_values[17] = Specular.x;
	_values[18] = Specular.y;
	_values[19] = Specular.z;
	_values[20] = Specular.w;
	_values[21] = AmbientStrenght;
	_values[22] = SpecularStrenght;
	_values[23] = Constant;
	_values[24] = Linear;
	_values[25] = Quadratic;
	Dirty = false;
}

void SpotLight::Save(std::string space, std::string& line)
{

#if JSON
	line += string_format(space + "\"Index\" : %d, \n", this->Index);
	line += string_format(space + "\"Ambient\" : [ %f, %f, %f, %f ],\n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "\"Diffuse\" : [ %f, %f, %f, %f ],\n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "\"Specular\" : [ %f, %f, %f, %f ],\n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "\"Direction\" : [ %f, %f, %f ], \n", this->Direction.x, this->Direction.y, this->Direction.z);
	line += string_format(space + "\"Parameters\" : [ %f, %f ] \n", this->CutOff, this->OuterCutOff);
	line += string_format(space + "\"Attenuation\" : [ %f, %f, %f ] \n", this->Constant, this->Linear, this->Quadratic);
#else
	line += string_format(space + "	Index : %d \n", this->Index);
	line += string_format(space + "	Ambient { %f, %f, %f, %f } \n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "	Diffuse { %f, %f, %f, %f } \n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "	Specular { %f, %f, %f, %f } \n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "	Direction { %f, %f, %f } \n", this->Direction.x, this->Direction.y, this->Direction.z);
	line += string_format(space + "	Parameters { %f, %f } \n", this->CutOff, this->OuterCutOff);
	line += string_format(space + "	Attenuation { %f, %f, %f } \n", this->Constant, this->Linear, this->Quadratic);
#endif
}

SpotLight* SpotLight::Load(const char* data, uint32_t& pos)
{
	SpotLight* spot = new SpotLight();
	while (1)
	{
		// Load Index.
		if (data[pos] == 'I')
		{
			spot->Index = Loader::GetInt(data, pos, 8);
		}
		// Load Ambient.
		else if (data[pos] == 'A' && data[pos + 1] == 'm')
		{
			spot->Ambient = Loader::GetVec4(data, pos, 10);
		}
		// Load Diffuse.
		else if (data[pos] == 'D' && data[pos + 1] == 'i' && data[pos + 2] == 'f')
		{
			spot->Diffuse = Loader::GetVec4(data, pos, 10);
		}
		// Load Specular.
		else if (data[pos] == 'S' && data[pos + 1] == 'p')
		{
			spot->Specular = Loader::GetVec4(data, pos, 11);
		}
		// Load Ambient strenght and specular strenght.
		else if (data[pos] == 'S' && data[pos + 1] == 't')
		{
			auto tmp = Loader::GetVec2(data, pos, 11);
			spot->AmbientStrenght = tmp.x;
			spot->SpecularStrenght = tmp.y;
		}
		//Load Direction.
		else if (data[pos] == 'D' && data[pos + 1] == 'i' && data[pos + 2] == 'r')
		{
			spot->Direction = Loader::GetVec3(data, pos, 12);
		}
		//Load Parameters.
		else if (data[pos] == 'P' && data[pos + 1] == 'a')
		{
			auto tmp = Loader::GetVec2(data, pos, 13);
			spot->CutOff = tmp.x;
			spot->OuterCutOff = tmp.y;
		}
		//Load Attenuation parameters.
		else if (data[pos] == 'A' && data[pos + 1] == 't')
		{
			auto tmp = Loader::GetVec3(data, pos, 14);
			spot->Constant = tmp.x;
			spot->Linear = tmp.y;
			spot->Quadratic = tmp.z;
			break;
		}
		else {
			pos++;
		}
	}
	app.GetLightManager()->AddLightComponent<SpotLight>(gameObject);
	spot->Dirty = true;
	return spot;
}

void SpotLight::ShowComponent()
{
	ImGui::ColorEdit3("Diffuse", &Diffuse[0]);
	ImGui::ColorEdit3("Ambient", &Ambient[0]);
	ImGui::ColorEdit3("Specular", &Specular[0]);
	ImGui::DragFloat("Ambient Strenght", &AmbientStrenght, 0.001f, 0, FLT_MAX);
	ImGui::DragFloat("Specular Strenght", &SpecularStrenght, 0.01f, 0, FLT_MAX);
	float euler = math::ToDegrees(CutOff);
	ImGui::DragFloat("Spot Angle", &euler, 0.5f, 0, FLT_MAX);
	CutOff = math::ToRadians(euler);
	euler = math::ToDegrees(OuterCutOff);
	ImGui::DragFloat("Out Spot Angle", &euler, 0.5f, 0, FLT_MAX);
	OuterCutOff = math::ToRadians(euler);
	ImGui::DragFloat3("Direction", &Direction[0], 0.01f, -1, 1);
	if (ImGui::BeginTable("Test", 3)) {
		ImGui::TableNextColumn();
		ImGui::Text("Constant");
		ImGui::TableNextColumn();
		ImGui::Text("Quadratic");
		ImGui::TableNextColumn();
		ImGui::Text("Linear");
		ImGui::TableNextColumn();
		ImGui::DragFloat("##1", &Constant, 0.01f, 0, FLT_MAX);
		ImGui::TableNextColumn();
		ImGui::DragFloat("##2", &Quadratic, 0.01f, 0, FLT_MAX);
		ImGui::TableNextColumn();
		ImGui::DragFloat("##3", &Linear, 0.01f, 0, FLT_MAX);
	}
	ImGui::EndTable();
	Dirty = true;
}

SpotLight::~SpotLight() {}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Lights, m) {
	py::class_<DirectionalLight, Component>(m, "DirectionalLight")
		.def_readwrite("Index", &DirectionalLight::Index)
		.def_readwrite("Ambient", &DirectionalLight::Ambient)
		.def_readwrite("Diffuse", &DirectionalLight::Diffuse)
		.def_readwrite("Specular", &DirectionalLight::Specular)
		.def_readwrite("Direction", &DirectionalLight::Direction)
		.def_readwrite("AmbientStrenght", &DirectionalLight::AmbientStrenght)
		.def_readwrite("SpecularStrenght", &DirectionalLight::SpecularStrenght);
	m.def("CreateDirLight", []() { return new DirectionalLight(); }, pybind11::return_value_policy::reference);


	py::class_<PointLight, Component>(m, "PointLight")
		.def_readwrite("Index", &PointLight::Index)
		.def_readwrite("Ambient", &PointLight::Ambient)
		.def_readwrite("Diffuse", &PointLight::Diffuse)
		.def_readwrite("Specular", &PointLight::Specular)
		.def_readwrite("AmbientStrenght", &PointLight::AmbientStrenght)
		.def_readwrite("SpecularStrenght", &PointLight::SpecularStrenght)
		.def_readwrite("Quadratic", &PointLight::Quadratic)
		.def_readwrite("Constant", &PointLight::Constant)
		.def_readwrite("Linear", &PointLight::Linear);
	m.def("CreatePtLight", []() { return new PointLight(); }, pybind11::return_value_policy::reference);

	py::class_<SpotLight, Component>(m, "SpotLight")
		.def_readwrite("Index", &SpotLight::Index)
		.def_readwrite("Ambient", &SpotLight::Ambient)
		.def_readwrite("Diffuse", &SpotLight::Diffuse)
		.def_readwrite("Specular", &SpotLight::Specular)
		.def_readwrite("Direction", &SpotLight::Direction)
		.def_readwrite("AmbientStrenght", &SpotLight::AmbientStrenght)
		.def_readwrite("SpecularStrenght", &SpotLight::SpecularStrenght)
		.def_readwrite("Quadratic", &SpotLight::Quadratic)
		.def_readwrite("Constant", &SpotLight::Constant)
		.def_readwrite("Linear", &SpotLight::Linear)
		.def_readwrite("CutOff", &SpotLight::CutOff)
		.def_readwrite("OuterCutOff", &SpotLight::OuterCutOff);
	m.def("CreateSptLight", []() { return new SpotLight(); }, pybind11::return_value_policy::reference);
}