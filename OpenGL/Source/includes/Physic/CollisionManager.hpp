#pragma once
#include "Colliders.hpp"

class CollisionManager
{
public:
	CollisionManager(GameState*);
	~CollisionManager();
	std::vector<Collider*> Colliders;
	void SphereSphereResponse(SphereCollider*, SphereCollider*, Vec3D);
	void SphereBoxResponse(SphereCollider* a, BoxCollider* b, Vec3D force);
	void Update();
private:
	GameState* _state;
};