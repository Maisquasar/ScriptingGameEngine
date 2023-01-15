#include "../Source/includes/Physic/CollisionManager.hpp"
#include "../Source/includes/Core/DataStructure/Node.hpp"

CollisionManager::CollisionManager(GameState* gs)
{
	_state = gs;
}

CollisionManager::~CollisionManager()
{
}



void DisableCollision(Collider* a, Collider* b)
{
	auto rb = a->GetRigidbody();
	if (rb && a->CollisionWith == b)
	{
		a->CollisionWith = nullptr;
		if (b->CollisionWith == a)
			b->CollisionWith = nullptr;
		//rb->enableGravity = true;
		rb->Grounded = false;
	}
}

void CollisionManager::Update()
{
	if (*_state != GameState::GS_PLAY)
		return;
	Vec3D dir;
	Vec3D force;
	for (int i = 0; i < Colliders.size(); i++)
	{
		if (Colliders[i] == nullptr) {
			if (Colliders[i]->Deleted)
				Colliders.erase(Colliders.begin() + i);
			continue;
		}
		for (int j = 0; j < Colliders.size(); j++)
		{
			if (Colliders[i]->Static)
				continue;
			if (Colliders[j] == nullptr || i == j || Colliders[i]->GetTransform() == Colliders[j]->GetTransform() || Colliders[j]->GetRigidbody() == nullptr && Colliders[i]->GetRigidbody() == nullptr) {
				if (Colliders[i]->Deleted)
					Colliders.erase(Colliders.begin() + i);
				continue;
			}
			switch (Colliders[i]->Type)
			{
			case ColliderType::BOX:
			{
				switch (Colliders[j]->Type)
				{
				case ColliderType::BOX: {
					if (IsBoxesCollide(Colliders[i], Colliders[j], dir))
					{
						if (auto rb = Colliders[i]->GetRigidbody()) {
							rb->Velocity.y = 0;
							//rb->enableGravity = false;
							rb->Contact(Colliders[j]->Friction);
							Colliders[i]->CollisionWith = Colliders[j];
							Colliders[j]->CollisionWith = Colliders[i];
						}
						Colliders[j]->Collide = true;
						Colliders[i]->Collide = true;
					}
					else
					{
						DisableCollision(Colliders[i], Colliders[j]);
					}
					break;
				}
				case ColliderType::SPHERE: {
					if (IsBoxSphereCollide(dynamic_cast<BoxCollider*>(Colliders[i]), dynamic_cast<SphereCollider*>(Colliders[j]), force))
					{
						SphereBoxResponse(dynamic_cast<SphereCollider*>(Colliders[j]), dynamic_cast<BoxCollider*>(Colliders[i]), force);
					}
					else
					{
						DisableCollision(Colliders[i], Colliders[j]);
					}
					break;
				}
				case ColliderType::CAPSULE: {
					break;
				}
				}
				break;
			}
			case ColliderType::SPHERE:
			{
				switch (Colliders[j]->Type)
				{
				case ColliderType::BOX: {
					if (IsBoxSphereCollide(dynamic_cast<BoxCollider*>(Colliders[j]), dynamic_cast<SphereCollider*>(Colliders[i]), force))
					{
						SphereBoxResponse(dynamic_cast<SphereCollider*>(Colliders[i]), dynamic_cast<BoxCollider*>(Colliders[j]), force);
					}
					else
					{
						DisableCollision(Colliders[i], Colliders[j]);
					}
					break;
				}
				case ColliderType::SPHERE:
				{
					if (IsSpheresCollide(Colliders[i], Colliders[j], force))
					{
						SphereSphereResponse(dynamic_cast<SphereCollider*>(Colliders[i]), dynamic_cast<SphereCollider*>(Colliders[j]), force);
					}
					else
					{
						DisableCollision(Colliders[i], Colliders[j]);
					}

					break;
				}
				case ColliderType::CAPSULE: {
					break;
				}
				}
				break;
			case ColliderType::CAPSULE:
			{
				switch (Colliders[j]->Type)
				{
				case ColliderType::BOX:
					break;
				case ColliderType::SPHERE:
					break;
				case ColliderType::CAPSULE:
					break;
				}
				break;
			default:
				break;
			}
			}
			}
		}
	}
}

void CollisionManager::SphereSphereResponse(SphereCollider* a, SphereCollider* b, Vec3D force)
{
	if (auto rb = a->GetRigidbody()) {

		rb->Velocity.y = 0;
		rb->Velocity += force;
		Vec3D dir = force.Normalize();
		auto normalized = rb->Velocity.Normalize();
		rb->GetTransform()->SetWorldPosition(rb->GetTransform()->GetWorldPosition() + force * 1.1f);
		if (dir.y <= 1 && dir.y > 0.7f) {
			rb->Grounded = true;
			//rb->enableGravity = false;
			rb->Velocity.y = 0;
		}
		else {
			rb->Velocity.x = (normalized.x + dir.x) * rb->Velocity.x * (rb->Velocity.x > 0 ? 1 : -1);
			rb->Velocity.z = (normalized.z + dir.z) * rb->Velocity.z * (rb->Velocity.z > 0 ? 1 : -1);
			rb->Grounded = false;
			//rb->enableGravity = true;
		}
		rb->Contact(b->Friction);
	}
	a->CollisionWith = b;
	b->CollisionWith = a;
	a->Collide = true;
	b->Collide = true;
}

void CollisionManager::SphereBoxResponse(SphereCollider* a, BoxCollider* b, Vec3D force)
{
	if (auto rb = a->GetRigidbody()) {
		//rb->Velocity += force;
		Vec3D dir = force.Normalize();
		auto normalized = rb->Velocity.Normalize();

		rb->GetTransform()->SetWorldPosition(rb->GetTransform()->GetWorldPosition() + force * 1.1f);
		if (dir.y <= 1 && dir.y > 0.7f) {
			rb->Grounded = true;
			////rb->enableGravity = false;
			rb->Velocity.y = 0;
		}
		else {
			rb->Velocity.x = (normalized.x + dir.x) * rb->Velocity.x * (rb->Velocity.x > 0 ? 1 : -1);
			rb->Velocity.z = (normalized.z + dir.z) * rb->Velocity.z * (rb->Velocity.z > 0 ? 1 : -1);
			rb->Grounded = false;
			////rb->enableGravity = true;
		}
		rb->Contact(b->Friction);
	}
	a->CollisionWith = b;
	b->CollisionWith = a;
	b->Collide = true;
	a->Collide = true;
}