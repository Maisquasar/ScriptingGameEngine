#pragma once
#include "../Resources/Component.hpp"
class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody();

	void ShowComponent() override;
	void GameUpdate() override;

	void Save(std::string space, std::string& line) override;
	Rigidbody* Load(const char* data, uint32_t& pos) override;
	void Initialize() override;

	void AddForce(Vec3D);
	void Contact(Vec3D);

	float Gravity = -9.81f;
	float Mass = 1.f;
	float Max = 10.f;
	Vec3D Friction = 1.f;
	Vec3D Velocity = Vec3D(0);
	bool enableGravity = true;

	bool Grounded = false;
private:
	void ApplyGravity(float);
};