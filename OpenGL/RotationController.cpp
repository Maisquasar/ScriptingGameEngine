#include "../Source/includes/Resources/Components/RotationController.hpp"
#include "../Source/includes/Core/App.hpp"
void RotationController::Save(std::string space, std::string& line)
{
#if JSON
	line += space + string_format("\"Axis\" : [ %d, %d, %d ],\n", Axis.x, Axis.y, Axis.z);
	line += space + string_format("\"Speed\" : [ %f, %f, %f ]\n", Speed.x, Speed.y, Speed.z);
#else
	line += space + string_format("Axis : { %d, %d, %d }\n", Axis.x, Axis.y, Axis.z);
	line += space + string_format("Speed : { %f, %f, %f }\n", Speed.x, Speed.y, Speed.z);
#endif
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_RotationController, m) {
	py::class_<RotationController, Component>(m, "RotationController")
		.def_readwrite("Axis", &RotationController::Axis)
		.def_readwrite("Speed", &RotationController::Speed);
	m.def("CreateRotationController", []() {return new RotationController(); }, py::return_value_policy::reference);
}
