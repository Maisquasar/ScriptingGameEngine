#pragma once
#include "../Resources/Component.hpp"
#include "../LowRenderer/Mesh.hpp"
#include "../Resources/Shader.hpp"
#include "../Physic/Rigidbody.hpp"

enum class ColliderType
{
	BOX,
	SPHERE,
	CAPSULE,
};

class Collider : public Component
{
public:
	Collider();
	~Collider();
	void Start() override {};
	virtual void Draw(Mat4D VP) {}
	virtual void Update(Mat4D VP) {}
	void SetShader(Shader* shader) { _shader = shader; }

	virtual std::vector<Vec3D> GetPoints();

	void SetRigidbody(Rigidbody* rb) { _rb = rb; }

	Collider* CollisionWith = nullptr;
	bool Collide = false;

	void PopulateNormals();
	void PopulatePoints();

	virtual Mat4D GetModelMatrix();
	virtual Vec3D GetWorldPosition();
	virtual Vec3D GetWorldScale();


	std::vector<Vec3D> GetTansformedNormals();
	virtual Rigidbody* GetRigidbody();
	Vec3D GetOffset();
	Vec3D GetFriction() { return this->Friction; }

	void SetFriction(Vec3D friction) { this->Friction = friction; }
	void SetOffset(Vec3D of) { this->_offset = of; }
	void SetShow(bool* ref);

	bool Deleted = false;
	Vec3D Friction = 1.f;
	bool Static = false;

	bool* show = nullptr;
	ColliderType Type = ColliderType::BOX;
	std::vector<Vec3D> _points;
protected:
	std::vector<float> _vertices;
	Shader* _shader = nullptr;
	GLuint _VBO = 0;
	GLuint _VAO = 0;

	std::vector<Vec3D> _transfromedNormals;
	std::vector<Vec3D> _transfromedPoints;

	std::vector<Vec3D> _normals;


	Vec3D _offset = Vec3D(0);

	Rigidbody* _rb = nullptr;
};

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();

	void Start() override;
	void Update(Mat4D VP);
	void Draw(Mat4D VP) override;
	void ShowComponent() override;

	void Save(std::string space, std::string& line) override;
	BoxCollider* Load(const char* data, uint32_t& pos) override;

	Mat4D GetModelMatrix() override;
	Mat4D GetLocalMatrix();

	std::vector<Vec3D> GetNormals();

	Vec3D GetSize();

	void SetSize(Vec3D size) { this->_size = size; }

	std::vector<Vec3D> Corner;
private:
	Vec3D _size = Vec3D(0.1f);

};

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();

	void Start() override;
	void Update(Mat4D VP);
	void Draw(Mat4D VP) override;
	void ShowComponent() override;

	void Save(std::string space, std::string& line) override;
	SphereCollider* Load(const char* data, uint32_t& pos) override;

	Mat4D GetModelMatrix() override;
	Vec3D GetWorldPosition() override;
	Vec3D GetWorldScale() override;

	float GetRadius() { return _radius; }
	void SetRadius(float rad) { _radius = rad; }
private:
	float _radius = 1.f;
};

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();
	~CapsuleCollider();

	void Start() override;
	void Update(Mat4D VP);
	void Draw(Mat4D VP) override;
	void ShowComponent() override;

	void Save(std::string space, std::string& line) override;
	CapsuleCollider* Load(const char* data, uint32_t& pos) override;

	float GetRadius() { return _radius; }
	float GetHeight() { return _height; }
	void SetRadius(float rad) { _radius = rad; }
	void SetHeight(float hei) { _height = _height; }

private:
	std::vector<float> _vertices[3];
	std::vector<float> _points[3];
	std::vector<float> _normals[3];
	GLuint _VAO[3] = { 0,0,0 };
	GLuint _VBO[3] = { 0,0,0 };
	float _radius = 1;
	float _height = 2;
	Vec3D _center = Vec3D(0);
	Vec3D _sphereLocalPos[2] = { 0 };
};

// Box Box Collision
bool IsBoxesCollide(Collider* shape1, Collider* shape2, Vec3D&);
// Sphere Sphere Collision
bool IsSpheresCollide(Collider* shape1, Collider* shape2, Vec3D&);
// Box Sphere Collision
bool IsBoxSphereCollide(BoxCollider* Box, SphereCollider* Sphere, Vec3D&);

bool GJK(Collider* shape1, Collider* shape2);