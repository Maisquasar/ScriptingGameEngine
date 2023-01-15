#include "../Source/includes/Resources/Material.hpp"
#include "../Source/includes/Core/App.hpp"

void Material::Save(std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Ambient\" : [ %f, %f, %f ],\n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "\"Diffuse\" : [ %f, %f, %f ],\n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "\"Specular\" : [ %f, %f, %f ],\n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "\"Shininess\" : %f,\n", this->Shininess);
	line += string_format(space + "\"Normal\" : \"%s\"\n", this->NormalMap.Name.c_str());
#else
	line += string_format(space + "Material %s\n", Name.c_str());
	line += string_format(space + "	Ambient { %f, %f, %f } \n", this->Ambient.x, this->Ambient.y, this->Ambient.z, this->Ambient.w);
	line += string_format(space + "	Diffuse { %f, %f, %f } \n", this->Diffuse.x, this->Diffuse.y, this->Diffuse.z, this->Diffuse.w);
	line += string_format(space + "	Specular { %f, %f, %f } \n", this->Specular.x, this->Specular.y, this->Specular.z, this->Specular.w);
	line += string_format(space + "	Shininess { %f } \n", this->Shininess);
	line += string_format(space + "	Normal %s\n", this->NormalMap.Name.c_str());
#endif
}

Material Material::Load(const char* data, uint32_t & pos)
{
	Material mat;
	mat.Name = Loader::GetString(data, pos, 9);
	while (1)
	{
		if (data[pos] == 'A')
		{
			mat.Ambient = Loader::GetVec3(data, pos, 10);
		}
		else if (data[pos] == 'D')
		{
			mat.Diffuse = Loader::GetVec3(data, pos, 10);
		}
		else if (data[pos] == 'S' && data[pos + 1] == 'p')
		{
			mat.Specular = Loader::GetVec3(data, pos, 11);
		}
		else if (data[pos] == 'S' && data[pos + 1] == 'h')
		{
			mat.Shininess = (float)Loader::GetInt(data, pos, 12);
		}
		else if (data[pos] == 'N')
		{
			std::string filename = Loader::GetString(data, pos, 7);
			if (auto tex = app.GetResourcesManager()->Get<Texture*>(filename))
				mat.NormalMap = *tex;
			break;
		}
		else {
			pos++;
		}
	}
	return mat;
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Material, m) {
	py::class_<Material>(m, "Material")
		.def(py::init<>())
		.def_readwrite("Name", &Material::Name)
		.def_readwrite("Ambient", &Material::Ambient)
		.def_readwrite("Diffuse", &Material::Diffuse)
		.def_readwrite("Specular", &Material::Specular)
		.def_readwrite("Shininess", &Material::Shininess)
		.def_readwrite("NormalMap", &Material::NormalMap)
		.def("SetNormalMap", &Material::SetNormalMap);
}