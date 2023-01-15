#include "../../includes/Resources/PythonScript.hpp"

PythonScript::PythonScript()
{
}

PythonScript::~PythonScript()
{
}

void PythonScript::Reload()
{
	try
	{
		if (_failed) {
			_script = pybind11::module::import(_name.c_str());
			_failed = false;
		}
		_script.reload();
		LOG(LogType::INFO, "File %s.py Reloaded", _name.c_str());
	}
	catch (const std::exception& exception)
	{
		_failed = true;
		LOG(LogType::L_ERROR, "File %s.py Fail Reload, Exception : %s", _name.c_str(), exception.what());
	}
}