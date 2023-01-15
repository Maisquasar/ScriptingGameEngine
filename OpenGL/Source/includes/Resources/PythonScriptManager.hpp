#pragma once
#include <pybind11/embed.h>
#include <Python/pyhelper.h>
#include <stdio.h>
#include "../Core/Debug/Log.hpp"
#include "PythonScript.hpp"

class PythonScriptManager
{
public:
	PythonScriptManager();
	~PythonScriptManager();

	void Initialize();

	void Load(const char* filename);
	void ReloadAll();

	PythonScript* GetScript(std::string name) { return _scripts[name]; }

private:
	std::map<std::string, PythonScript*> _scripts;
};
