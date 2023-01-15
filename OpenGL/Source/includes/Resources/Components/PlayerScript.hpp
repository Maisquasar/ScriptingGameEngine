#pragma once
#include "../Component.hpp"
#include "../../LowRenderer/Camera.hpp"
#include "../../Physic/Rigidbody.hpp"
struct PlayerInput
{
	bool forward = false;
	bool backward = false;
	bool left = false;
	bool right = false;
	bool jump = false;
};

static const char* KeysName[] = { "Forward", "Backward", "Left", "Right", "Jump" };

class PlayerScript : public Component
{
public:
	PlayerScript()
	{
		ComponentName = "PlayerScript";
	}
	PlayerScript(Rigidbody* rb)
	{
		ComponentName = "PlayerScript";
		_rb = rb;
	}
	PlayerScript(Camera* camera, Rigidbody* rb)
	{
		ComponentName = "PlayerScript";
		_cam = camera;
		_rb = rb;
	}
	~PlayerScript();

	std::map<std::string, int> Key;

	void InitKeys();

	void Start() override;
	void GameUpdate() override;
	void ShowComponent() override;
	void SetInput(PlayerInput input) { _input = input; }
	void SetCamera(Camera* cam) { _cam = cam; }
	void SetRigidbody(Rigidbody* rb) { _rb = rb; }

	PlayerInput GetInput() { return _input; }
	Rigidbody* GetRigidbody() { return _rb; }
	Camera* GetCamera() { return _cam; }

	void Save(std::string space, std::string& line) override;
	PlayerScript* Load(const char* data, uint32_t& pos) override;

	float Speed = 5.f;
	float JumpForce = 5.f;
	void InputUpdate();
private:
	PlayerInput _input = { 0 };
	Camera* _cam = nullptr;
	Rigidbody* _rb = nullptr;
};