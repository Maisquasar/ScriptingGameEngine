#include "../Source/includes/Resources/Components/CameraLock.hpp"
#include "../Source/includes/Core/App.hpp"
void CameraLock::Start()
{
	app.camera.Lockposition = this;
}


void CameraLock::Save(std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Enable\" : %d\n", this->enable);
#else
#endif // 0

}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_CameraLock, m) {
	py::class_<CameraLock, Component>(m, "CameraLock");

	m.def("CreateCameraLock", []() { return new CameraLock(); }, pybind11::return_value_policy::reference);
}