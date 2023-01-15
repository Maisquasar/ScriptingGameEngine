#pragma once
#include "../Component.hpp"
class CameraLock : public Component
{
public:
	CameraLock() 
	{
		ComponentName = "Camera Lock";
	}
	~CameraLock() {}
	void ShowComponent() override
	{
	}

	void Start() override;

	void Save(std::string space, std::string& line);

	CameraLock* Load(const char* data, uint32_t& pos) override
	{
		CameraLock* CL = new CameraLock();
		return CL;
	}
private:
};