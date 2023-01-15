#pragma once
#include <pybind11/pybind11.h>
#include <string>
#include <ImGui/imgui.h>
#include "../Core/Debug/Log.hpp"

class PythonScript
{
public:
	friend class PythonScriptManager;

	PythonScript();
	~PythonScript();

	template<typename ...Args>
	void ExecuteFunction(std::string name, Args ... args) {
		try
		{
			if (!_failed) {
				if (!functions[name])
					functions[name] = new pybind11::detail::str_attr_accessor(_script.attr(name.c_str()));
				(*functions[name])(args ...);
			}
		}
		catch (const std::exception& exception)
		{
			LOG(LogType::L_ERROR, "Function %s Not found in %s.py, Exception : %s", name.c_str(), this->_name.c_str(), exception.what());
		}
	}

	void Reload();
private:
	std::map<std::string, pybind11::detail::str_attr_accessor*> functions;
	bool _failed = false;
	pybind11::module _script;
	std::string _name;
};