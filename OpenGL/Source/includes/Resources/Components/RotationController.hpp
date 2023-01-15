#pragma once
#include "../../LowRenderer/Transform.hpp"
#include "../Component.hpp"


class RotationController : public Component
{
public:
	RotationController() 
	{
		ComponentName = "Rotation Controller";
	}
	~RotationController() {}

	Int3D Axis = { 0 };
	Vec3D Speed = { 0 };

	// ImGui Draw.
	void ShowComponent() override
	{
		ImGui::DragInt3("Rotation Axis", &Axis.x, 1, -1, 1);
		ImGui::NewLine();
		ImGui::DragFloat3("Speed", &Speed.x, 0.1f, 0, FLT_MAX);
	}

	void GameUpdate() override
	{
		// Apply rotation at the axis and the speed.
		auto tmp = GetTransform()->GetLocalRotation() + Vec3D(math::Mod(Axis.x * Speed.x * ImGui::GetIO().DeltaTime, 360), math::Mod(Axis.y * Speed.y * ImGui::GetIO().DeltaTime, 360), math::Mod(Axis.z * Speed.z * ImGui::GetIO().DeltaTime, 360));
		GetTransform()->SetLocalRotation(tmp);
		GetTransform()->SetDirty(true);
	}

	// Save and Load.
	void Save(std::string space, std::string& line) override;

	RotationController* Load(const char* data, uint32_t& pos) override
	{
		RotationController* RC = new RotationController();
		while (1)
		{
			if (data[pos] == 'A')
			{
				RC->Axis = Loader::GetVec3(data, pos, 9);
			}
			else if (data[pos] == 'S')
			{
				RC->Speed = Loader::GetVec3(data, pos, 10);
				break;
			}
			else {
				pos++;
			}
		}
		return RC;
	}
};