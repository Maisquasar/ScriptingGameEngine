#include "../Source/includes/Resources/PythonScriptManager.hpp"


PythonScriptManager::PythonScriptManager()
{
}

PythonScriptManager::~PythonScriptManager()
{
}

void PythonScriptManager::Initialize()
{
	Py_Initialize();
	std::string str2 = (std::string)(std::filesystem::current_path().generic_string() + "/Source/scripts");
	std::replace(str2.begin(), str2.end(), '/', '\\');
	char buf[128];
	snprintf(buf, 128, "sys.path[3] = r'%s'", str2.c_str());
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(buf);
}


void PythonScriptManager::Load(const char* filename)
{
	_scripts[filename] = new PythonScript();
	try
	{
		auto pModule = pybind11::module::import(filename);
		_scripts[filename]->_script = pModule;
		_scripts[filename]->_name = filename;
		if (!_scripts[filename]->_script)
			LOG(LogType::L_ERROR, "Module %s Not found", filename);
		if (_scripts[filename]->_script)
			LOG(LogType::INFO, "Module %s Loaded", filename);
	}
	catch (const std::exception& exception)
	{

		LOG(LogType::L_ERROR, "File %s Failed To compile, Exception : %s", filename, exception.what());
		_scripts[filename]->_name = std::string(filename);
		_scripts[filename]->_failed = true;

	}
}

void PythonScriptManager::ReloadAll()
{
	for (auto i : _scripts)
	{
		if (!i.second)
			continue;
		i.second->Reload();
	}
	LOG(LogType::INFO, "All File Reloaded");
}

