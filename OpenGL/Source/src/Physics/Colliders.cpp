#include "../../includes/Physic/Colliders.hpp"
#include "../../includes/Core/App.hpp"
Collider::Collider()
{
}

Collider::~Collider()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

void Collider::SetShow(bool* ref)
{
	show = ref;
}

void Collider::PopulateNormals()
{
	_transfromedNormals.clear();
	for (int i = 0; i < _normals.size(); i++)
	{
		_transfromedNormals.push_back(GetModelMatrix() * Vec4D(_normals[i], 0));
	}
}

void Collider::PopulatePoints()
{
	_transfromedPoints.clear();
	for (int i = 0; i < _points.size(); i++)
	{
		_transfromedPoints.push_back(GetModelMatrix() * Vec4D(this->_points[i], 1));
	}
}


std::vector<Vec3D> Collider::GetPoints()
{
	return _transfromedPoints;
}
std::vector<Vec3D> Collider::GetTansformedNormals()
{
	return _transfromedNormals;
}

Rigidbody* Collider::GetRigidbody() { return _rb; }
Mat4D Collider::GetModelMatrix() { return Mat4D::Identity(); }
Vec3D Collider::GetWorldPosition() { return Vec3D(); }
Vec3D Collider::GetWorldScale() { return Vec3D(); }
Vec3D Collider::GetOffset() { return _offset; }

// ----- Box Collider -----

BoxCollider::BoxCollider()
{
	ComponentName = "Box Collider";
}

BoxCollider::~BoxCollider()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

void BoxCollider::Start()
{
	app.GetColliderManager()->Colliders.push_back(this);
	this->_shader = dynamic_cast<Shader*>(GetResourcesManager()->at("Source/shaders/NoLightsShader"));
	this->SetShow(&app.settings.ShowCollision);
	this->Type = ColliderType::BOX;

	_vertices = math::GetCubeVertices(_points, _normals);
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(POS);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float[3])));
	glEnableVertexAttribArray(2);
	PopulateNormals();
	PopulatePoints();
	if (auto rb = gameObject->GetComponent<Rigidbody>())
		this->SetRigidbody(rb);
}

void BoxCollider::Update(const Mat4D VP)
{
	if (GetTransform()->IsDirty())
	{
		PopulateNormals();
		PopulatePoints();
	}
	if (*show)
		Draw(VP);
}

void BoxCollider::Draw(Mat4D VP)
{
	glUseProgram(_shader->Program);
	// Set the Model Matrix.
	Mat4D M = Mat4D::CreateTransformMatrix(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldRotation(), 1) * Mat4D::CreateTransformMatrix(this->_offset, 0, this->_size);
	Mat4D MVP = VP * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	if (!Collide)
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);
	else
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 1, 0, 0, 1);
	glUniform1i(_shader->GetLocation(Location::L_ENABLE_TEXTURE), false);
	int polygonMode;
	// Enable Wireframe.
	glDisable(GL_CULL_FACE);
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices.size());

	// Disable Wireframe.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	Collide = false;
}

void BoxCollider::ShowComponent()
{
	ImGui::DragFloat3("Size", &_size.x, (float)0.01, 0, FLT_MAX);
	ImGui::SameLine();
	if (ImGui::Button("Fit"))
	{
		_size = GetTransform()->GetLocalScale();
	}
	ImGui::DragFloat3("Offset", &_offset.x, (float)0.01);
	ImGui::DragFloat3("Friction", &Friction.x, 0.01f, 0);
	ImGui::Checkbox("Static", &Static);
}

void BoxCollider::Save(const std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Size\" : [ %f, %f, %f ],\n", this->_size.x, this->_size.y, this->_size.z);
	line += string_format(space + "\"Offset\" : [ %f, %f, %f ],\n", this->_offset.x, this->_offset.y, this->_offset.z);
	line += string_format(space + "\"Friction\" : [ %f, %f, %f ],\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "\"Static\" : %d\n", this->Static);
#else
	line += string_format(space + "	Size { %f, %f, %f } \n", this->_size.x, this->_size.y, this->_size.z);
	line += string_format(space + "	Offset { %f, %f, %f } \n", this->_offset.x, this->_offset.y, this->_offset.z);
	line += string_format(space + "	Friction { %f, %f, %f }\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "	Static : %d\n", this->Static);
#endif
}

BoxCollider* BoxCollider::Load(const char* data, uint32_t& pos)
{
	BoxCollider* box = new BoxCollider();
	while (1)
	{
		if (data[pos] == 'S' && data[pos + 1] == 'i')
		{
			box->_size = Loader::GetVec3(data, pos, 7);
		}
		else if (data[pos] == 'O')
		{
			box->_offset = Loader::GetVec3(data, pos, 9);
		}
		else if (data[pos] == 'F')
		{
			box->Friction = Loader::GetVec3(data, pos, 11);
		}
		else if (data[pos] == 'S') 
		{
			box->Static = Loader::GetInt(data, pos, 9);
			break;
		}
		else {
			pos++;
		}
	}
	return box;
}

Mat4D BoxCollider::GetModelMatrix()
{
	return GetTransform()->GetModelMatrix().Multiply(Mat4D::CreateTransformMatrix(_offset, 0, _size));
}

Mat4D BoxCollider::GetLocalMatrix()
{
	return Mat4D::CreateTransformMatrix(GetTransform()->GetLocalPosition() + _offset, GetTransform()->GetLocalRotation(), GetTransform()->GetLocalScale() + _size);
}

std::vector<Vec3D> BoxCollider::GetNormals() { return _normals; }

Vec3D BoxCollider::GetSize() { return _size; }

// ----- Sphere Collider -----

SphereCollider::SphereCollider()
{
	ComponentName = "Sphere Collider";
}

SphereCollider::~SphereCollider()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

void SphereCollider::Start()
{
	app.GetColliderManager()->Colliders.push_back(this);
	this->_shader = dynamic_cast<Shader*>(GetResourcesManager()->at("Source/shaders/NoLightsShader"));
	this->SetShow(&app.settings.ShowCollision);
	this->Type = ColliderType::SPHERE;

	// Set To Collider Manager.
	app.GetColliderManager()->Colliders.push_back(dynamic_cast<SphereCollider*>(this));

	_vertices = math::GetSphereVertices(_points, _normals, 1);
	//_vertices = math::GetCapVertices(_points, _normals, 1, 1.5);

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(POS);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float[3])));
	glEnableVertexAttribArray(2);
}

void SphereCollider::Update(const Mat4D VP)
{
	//if (!*_IsInplay)
	if (*show)
		Draw(VP);
}

Mat4D SphereCollider::GetModelMatrix()
{
	return GetTransform()->GetModelMatrix().Multiply(Mat4D::CreateTransformMatrix(_offset, Vec3D(0), _radius));
}

void SphereCollider::Draw(Mat4D VP)
{
	glUseProgram(_shader->Program);
	// Set the Model Matrix.
	Mat4D M = GetModelMatrix();
	Mat4D MVP = VP * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	if (!Collide)
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);
	else
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 1, 0, 0, 1);
	glUniform1i(_shader->GetLocation(Location::L_ENABLE_TEXTURE), false);
	int polygonMode;
	// Enable Wireframe.
	glDisable(GL_CULL_FACE);
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices.size());
	// Disable Wireframe.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	Collide = false;
	if (auto rb = gameObject->GetComponent<Rigidbody>())
		this->SetRigidbody(rb);
}

void SphereCollider::ShowComponent()
{
	ImGui::DragFloat("Radius", &_radius, (float)0.01, 0, FLT_MAX);
	ImGui::DragFloat3("Center", &_offset.x, (float)0.01);
	ImGui::DragFloat3("Friction", &Friction.x, 0.01f, 0);
	ImGui::Checkbox("Static", &Static);
}

void SphereCollider::Save(const std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Radius\" : %f,\n", this->_radius);
	line += string_format(space + "\"Center\" : [ %f, %f, %f ],\n", this->_offset.x, this->_offset.y, this->_offset.z);
	line += string_format(space + "\"Friction\" : [ %f, %f, %f ],\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "\"Static\" : %d\n", this->Static);
#else
	line += string_format(space + "	Radius : %f\n", this->_radius);
	line += string_format(space + "	Center { %f, %f, %f } \n", this->_offset.x, this->_offset.y, this->_offset.z);
	line += string_format(space + "	Friction { %f, %f, %f }\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "	Static : %d\n", this->Static);
#endif
}

SphereCollider* SphereCollider::Load(const char* data, uint32_t& pos)
{
	SphereCollider* sph = new SphereCollider();
	while (1)
	{
		if (data[pos] == 'R')
		{
			sph->_radius = Loader::GetFloat(data, pos, 9);
		}
		else if (data[pos] == 'C')
		{
			sph->_offset = Loader::GetVec3(data, pos, 9);
		}
		else if (data[pos] == 'F')
		{
			sph->Friction = Loader::GetVec3(data, pos, 11);
		}
		else if (data[pos] == 'S')
		{
			sph->Static = Loader::GetInt(data, pos, 9);
			break;
		}
		else {
			pos++;
		}
	}
	return sph;
}

Vec3D SphereCollider::GetWorldPosition() { return GetModelMatrix().GetPosition(); }
Vec3D SphereCollider::GetWorldScale() { return GetModelMatrix().GetScale(); }


// ----- Capsule Collider -----


CapsuleCollider::CapsuleCollider()
{
	ComponentName = "Capsule Collider";
}
CapsuleCollider::~CapsuleCollider() {}

void CapsuleCollider::Start()
{
	app.GetColliderManager()->Colliders.push_back(this);
	this->_shader = dynamic_cast<Shader*>(GetResourcesManager()->at("Source/shaders/NoLightsShader"));
	this->SetShow(&app.settings.ShowCollision);
	this->Type = ColliderType::CAPSULE;


	_vertices[0] = math::GetDemiSphereVertices(_radius, true);
	_vertices[1] = math::GetCylindreVertices(_radius, 1.f);
	_vertices[2] = math::GetDemiSphereVertices(_radius, false);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	for (int i = 0; i < 3; i++)
	{
		glGenVertexArrays(1, &_VAO[i]);
		glGenBuffers(1, &_VBO[i]);
		glBindVertexArray(_VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices[i].size(), _vertices[i].data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(POS);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(2);
	}
	if (auto rb = gameObject->GetComponent<Rigidbody>())
		this->SetRigidbody(rb);
}
void CapsuleCollider::Update(const Mat4D VP)
{
	//if (!*_IsInplay)
	if (*show)
		Draw(VP);
}
void CapsuleCollider::Draw(Mat4D VP)
{
	glUseProgram(_shader->Program);
	// Draw Up Demi Sphere.
	// Set the Model Matrix.
	Mat4D M = GetTransform()->GetModelMatrix().Multiply(Mat4D::CreateTransformMatrix(_center + Vec3D(0, _height / 2, 0), Vec3D(0), _radius));
	Mat4D MVP = VP * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	if (!Collide)
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);
	else
		glUniform4f(_shader->GetLocation(Location::L_COLOR), 1, 0, 0, 1);


	int polygonMode;
	// Enable Wireframe.
	glDisable(GL_CULL_FACE);
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(_VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices[0].size());

	// Draw Cylindre.
	M = GetTransform()->GetModelMatrix().Multiply(Mat4D::CreateTransformMatrix(_center, Vec3D(0), Vec3D(_radius, _height, _radius)));
	MVP = VP * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);
	glUniform1i(_shader->GetLocation(Location::L_ENABLE_TEXTURE), false);

	glBindVertexArray(_VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices[1].size());

	// Draw Down Sphere.
	M = GetTransform()->GetModelMatrix().Multiply(Mat4D::CreateTransformMatrix(_center - Vec3D(0, _height / 1.52f, 0), Vec3D(0), _radius));
	MVP = VP * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);

	glBindVertexArray(_VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices[2].size());

	//Disable Wireframe.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	Collide = false;
}
void CapsuleCollider::ShowComponent()
{
	ImGui::DragFloat("Radius", &_radius, (float)0.01, 0, FLT_MAX);
	ImGui::DragFloat("Height", &_height, (float)0.01, 0, FLT_MAX);
	ImGui::DragFloat3("Center", &_center.x, (float)0.01);
	ImGui::DragFloat3("Friction", &Friction.x, 0.01f, 0);
	ImGui::Checkbox("Static", &Static);
}

void CapsuleCollider::Save(const std::string space, std::string& line)
{
#if JSON
	line += string_format(space + "\"Radius\" : %f,\n", this->_radius);
	line += string_format(space + "\"Height\" : %f,\n", this->_height);
	line += string_format(space + "\"Center\" : [ %f, %f, %f ],\n", this->_center.x, this->_center.y, this->_center.z);
	line += string_format(space + "\"Friction\" : [ %f, %f, %f ],\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "\"Static\" : %d\n", this->Static);
#else
	line += string_format(space + "	Radius : %f\n", this->_radius);
	line += string_format(space + "	Height : %f\n", this->_height);
	line += string_format(space + "	Center { %f, %f, %f } \n", this->_center.x, this->_center.y, this->_center.z);
	line += string_format(space + "	Friction { %f, %f, %f }\n", this->Friction.x, this->Friction.y, this->Friction.y);
	line += string_format(space + "	Static : %d\n", this->Static);
#endif
}
CapsuleCollider* CapsuleCollider::Load(const char* data, uint32_t& pos)
{
	CapsuleCollider* cap = new CapsuleCollider();
	while (1)
	{
		if (data[pos] == 'R')
		{
			cap->_radius = Loader::GetFloat(data, pos, 9);
		}
		else if (data[pos] == 'C')
		{
			cap->_center = Loader::GetVec3(data, pos, 9);
			break;
		}
		else if (data[pos] == 'R')
		{
			cap->_height = Loader::GetFloat(data, pos, 9);
		}
		else if (data[pos] == 'F')
		{
			cap->Friction = Loader::GetVec3(data, pos, 11);
		}
		else if (data[pos] == 'S')
		{
			cap->Static = Loader::GetInt(data, pos, 9);
			break;
		}
		else {
			pos++;
		}
	}
	return cap;
}

#pragma region BOX x BOX

inline bool IsBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}

bool Overlaps(float min1, float max1, float min2, float max2)
{
	return IsBetweenOrdered(min2, min1, max1) || IsBetweenOrdered(min1, min2, max2);
}


void SATtest(Vec3D& axis, std::vector<Vec3D>& ptSet, float& minAlong, float& maxAlong)
{
	minAlong = (float)HUGE, maxAlong = (float)-HUGE;
	for (int i = 0; i < ptSet.size(); i++)
	{
		// just dot it to get the min/max along this axis.
		float dotVal = ptSet[i].DotProduct(axis);
		if (dotVal < minAlong)  minAlong = dotVal;
		if (dotVal > maxAlong)  maxAlong = dotVal;
	}
}

bool IsBoxesCollide(Collider* shape1, Collider* shape2, Vec3D& dir)
{
	//TODO : Return the good direction.
	Vec3D tmp;
	auto points1 = shape1->GetPoints();
	auto points2 = shape2->GetPoints();
	// Get the normals for one of the shapes,
	bool intersect = false;
	auto normals = shape1->GetTansformedNormals();
	for (int i = 0; i < normals.size(); i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(normals[i], points1, shape1Min, shape1Max);
		SATtest(normals[i], points2, shape2Min, shape2Max);
		if (!Overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return 0;
		}
	}
	normals = shape2->GetTansformedNormals();
	for (int i = 0; i < normals.size(); i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(normals[i], points2, shape1Min, shape1Max);
		SATtest(normals[i], points1, shape2Min, shape2Max);
		if (!Overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return 0;
		}
	}
	dir = tmp.Normalize();
	return 1;
}
#pragma endregion

#pragma region SPHERE X SPHERE
bool IsSpheresCollide(Collider* shape1, Collider* shape2, Vec3D& force)
{
	auto pos1 = shape1->GetWorldPosition();
	auto pos2 = shape2->GetWorldPosition();
	auto radius1 = shape1->GetWorldScale();
	auto radius2 = shape2->GetWorldScale();
	float distance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z));

	//dir = Vec3D(sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)), sqrtf((pos1.y - pos2.y) * (pos1.y - pos2.y)), sqrtf((pos1.z - pos2.z) * (pos1.z - pos2.z)));
	//dir = Vec3D(radius1.x + radius2.x) - dir;
	Vec3D dir = pos1 - pos2;
	dir = dir.Normalize();
	float dis = radius1.x + radius2.x - distance;
	force = dir * dis;
	// Distance
	return distance < (radius1.x + radius2.x);
}
#pragma endregion


// Returns the squared distance between a point p and an AABB b
float SqDistPointAABB(SphereCollider* Sphere, BoxCollider* Box, Vec3D& force)
{
	if (Box->GetModelMatrix().GetPosition().GetDistanceBetween(Sphere->GetWorldPosition()) > 20.f)
		return FLT_MAX;

	Vec3D rot = Box->GetModelMatrix().GetRotation() * -1;
	Vec3D Size = Box->GetSize();
	Vec3D bPos = Box->GetModelMatrix().GetPosition();
	Mat4D mat = Mat4D::CreateTransformMatrix(bPos, rot, 1);
	Vec3D pos = mat * Sphere->GetModelMatrix().GetPosition();
	Vec3D AworldPos = mat * bPos;

	float r = Sphere->GetWorldScale().x;

	mat = Mat4D::CreateTransformMatrix(AworldPos, 0, 1);
	Vec3D aMax = mat * (Size);
	Vec3D aMin = mat * (Size.Negate());

	float sqDist = 0.0f;
	Vec3D tmp;

	for (int i = 0; i < 3; i++) {
		// for each axis count any excess distance outside box extents
		float v = pos[i];
		if (v < aMin[i]) {
			tmp[i] = (aMin[i] - v) * (aMin[i] - v);
			sqDist += tmp[i];
		}
		if (v > aMax[i]) {
			tmp[i] = (v - aMax[i]) * (v - aMax[i]);
			sqDist += tmp[i];
		};
	}
	//tmp.Print();
	force = Vec3D(0);
	if (tmp.x > 0.1f)
		force += Vec3D(r - tmp.x, 0, 0);
	if (tmp.y > 0.1f)
		force += Vec3D(0, r - tmp.y, 0);
	if (tmp.z > 0.1f)
		force += Vec3D(0, 0, r - tmp.z);
	if (force.x > 0 && force.z > 0 || force.x > 0 && force.y > 0 || force.z > 0 && force.y > 0)
		force = force * 0.01f;

	Vec3D dir = (pos - AworldPos).Normalize();
	force = dir * force;
	return sqDist;
}

bool IsBoxSphereCollide(BoxCollider* Box, SphereCollider* Sphere, Vec3D& force) {
	float r = Sphere->GetWorldScale().x;

	// Compute squared distance between sphere center and AABB
	// the sqrt(dist) is fine to use as well, but this is faster.
	float sqDist = SqDistPointAABB(Sphere, Box, force);

	// Sphere and AABB intersect if the (squared) distance between them is
	// less than the (squared) sphere radius.
	return sqDist <= r * r;
}


std::vector<Vec3D> GetSuport(Collider* shape1, Collider* shape2, Vec3D& d)
{
	//shape1.
	std::vector<Vec3D> a;
	return a;
}
bool GJK(Collider* shape1, Collider* shape2)
{
	Vec3D d = (shape2->GetOffset() - shape1->GetOffset()).Normalize();
	std::vector<Vec3D> suportPoints = GetSuport(shape1, shape2, d);
	return false;
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Colliders, m) {
	py::class_<BoxCollider, Component>(m, "BoxCollider")
		.def_readwrite("Static", &BoxCollider::Static)
		.def_property("Size", &BoxCollider::GetSize, &BoxCollider::SetSize)
		.def_property("Offset", &BoxCollider::GetOffset, &BoxCollider::SetOffset)
		.def_property("Friction", &BoxCollider::GetFriction, &BoxCollider::SetFriction);
	m.def("CreateBoxCollider", []() { return new BoxCollider(); }, pybind11::return_value_policy::reference);

	py::class_<SphereCollider, Component>(m, "SphereCollider")
		.def_readwrite("Static", &SphereCollider::Static)
		.def_property("Radius", &SphereCollider::GetRadius, &SphereCollider::SetRadius)
		.def_property("Offset", &SphereCollider::GetOffset, &SphereCollider::SetOffset)
		.def_property("Friction", &SphereCollider::GetFriction, &SphereCollider::SetFriction);
	m.def("CreateSphereCollider", []() { return new SphereCollider(); }, pybind11::return_value_policy::reference);

	py::class_<CapsuleCollider, Component>(m, "CapsuleCollider")
		.def_readwrite("Static", &CapsuleCollider::Static)
		.def_property("Radius", &CapsuleCollider::GetRadius, &CapsuleCollider::SetRadius)
		.def_property("Height", &CapsuleCollider::GetHeight, &CapsuleCollider::SetHeight)
		.def_property("Offset", &CapsuleCollider::GetOffset, &CapsuleCollider::SetOffset)
		.def_property("Friction", &CapsuleCollider::GetFriction, &CapsuleCollider::SetFriction);
	m.def("CreateCapsuleCollider", []() { return new CapsuleCollider(); }, pybind11::return_value_policy::reference);
}