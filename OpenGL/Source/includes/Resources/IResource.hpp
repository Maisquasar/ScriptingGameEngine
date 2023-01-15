#pragma once
#include <iostream>
#include <thread>
#include "../Core/Maths/Maths.hpp"

class IResource
{
public:
	std::string R_Name;
	bool Loaded;

	IResource();
	IResource(const IResource&) {}
	virtual IResource operator=(const IResource& a) { return *this; }
	virtual ~IResource();
	virtual void ShowResource() { ImGui::Text("..."); };
	virtual bool IsLoaded() { return Loaded; }
};