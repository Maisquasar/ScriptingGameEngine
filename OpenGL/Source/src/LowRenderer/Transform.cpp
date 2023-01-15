#include "../../includes/LowRenderer/Transform.hpp"
#include "../../includes/Core/App.hpp"

Mat4D Transform::GetLocalModelMatrix()
{
	return Mat4D::CreateTransformMatrix(_position, _rotation, _scale);
}

void Transform::ComputeModelMatrix()
{
	ModelMatrix = GetLocalModelMatrix();
	SetDirty(false);
}

void Transform::ComputeModelMatrix(Mat4D parentGlobalModelMatrix)
{
	ModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
	SetDirty(false);
}

void Transform::SetLocalTransform(const Vec3D& newPosition, const Vec3D& newRotation, const Vec3D& newScale)
{
	_position = newPosition;
	_rotation = newRotation;
	_scale = newScale;
	_isDirty = true;
}

void Transform::SetWorldTransform(const Vec3D& newPosition, const Vec3D& newRotation, const Vec3D& newScale)
{
	SetWorldPosition(newPosition);
	SetWorldRotation(newRotation);
	SetWorldScale(newScale == 0 ? GetWorldScale() : newScale);
}

void Transform::SetWorldPosition(const Vec3D& newPosition)
{
	_position = newPosition;
	ModelMatrix.content[0][3] = newPosition.x;
	ModelMatrix.content[1][3] = newPosition.y;
	ModelMatrix.content[2][3] = newPosition.z;
}

void Transform::SetWorldRotation(const Vec3D& newRotation)
{
	ModelMatrix = Mat4D::CreateTransformMatrix(GetWorldPosition(), newRotation, GetWorldScale());
}

void Transform::SetWorldScale(const Vec3D& newScale)
{
	ModelMatrix = Mat4D::CreateTransformMatrix(GetWorldPosition(), GetWorldRotation(), newScale);
}

void Transform::SetLocalPosition(const Vec3D& newPosition)
{
	_position = newPosition;
	_isDirty = true;
}
void Transform::SetLocalRotation(const Vec3D& newRotation)
{
	_rotation = newRotation;
	_isDirty = true;
}
void Transform::SetLocalScale(const Vec3D& newScale)
{
	_scale = newScale;
	_isDirty = true;
}

const Vec3D& Transform::GetLocalPosition()
{
	return _position;
}
const Vec3D& Transform::GetLocalRotation()
{
	return _rotation;
}
const Vec3D& Transform::GetLocalScale()
{
	return _scale;
}

const Mat4D& Transform::GetModelMatrix()
{
	return ModelMatrix;
}
const Vec3D Transform::GetWorldPosition()
{
	return Vec3D(ModelMatrix.content[0][3], ModelMatrix.content[1][3], ModelMatrix.content[2][3]);
}
const Vec3D Transform::GetWorldRotation()
{
	Vec3D sca = GetWorldScale();
	Mat4D rotMat;
	// Get Rotation Matrix.
	if (sca == Vec3D(0))
		sca = Vec3D(0.0001f);
	rotMat.at(0, 0) = ModelMatrix.at(0, 0) / sca.x;
	rotMat.at(0, 1) = ModelMatrix.at(0, 1) / sca.x;
	rotMat.at(0, 2) = ModelMatrix.at(0, 2) / sca.x;
	rotMat.at(1, 0) = ModelMatrix.at(1, 0) / sca.y;
	rotMat.at(1, 1) = ModelMatrix.at(1, 1) / sca.y;
	rotMat.at(1, 2) = ModelMatrix.at(1, 2) / sca.y;
	rotMat.at(2, 0) = ModelMatrix.at(2, 0) / sca.z;
	rotMat.at(2, 1) = ModelMatrix.at(2, 1) / sca.z;
	rotMat.at(2, 2) = ModelMatrix.at(2, 2) / sca.z;
	rotMat.at(3, 3) = 1;

	// Get Rotation from rotation matrix.
	float thetaX, thetaY, thetaZ;
	if (rotMat.at(2, 1) < 1)
	{
		if (rotMat.at(2, 1) > -1)
		{
			thetaX = asin(-rotMat.at(2, 1));
			thetaY = atan2(rotMat.at(2, 0), rotMat.at(2, 2));
			thetaZ = atan2(rotMat.at(0, 1), rotMat.at(1, 1));
		}
		else
		{
			thetaX = (float)PI / 2;
			thetaY = -atan2(rotMat.at(1, 0), rotMat.at(0, 0));
			thetaZ = 0;
		}
	}
	else
	{
		thetaX = -PI / 2;
		thetaY = atan2(rotMat.at(1, 0), rotMat.at(0, 0));
		thetaZ = 0;
	}
	return Vec3D(thetaX, thetaY, thetaZ).ToDeegree();
}
const Vec3D Transform::GetWorldScale()
{
	// World Scale equal lenght of columns of the model matrix.
	float x = Vec3D(ModelMatrix.content[0][0], ModelMatrix.content[0][1], ModelMatrix.content[0][2]).GetLength();
	float y = Vec3D(ModelMatrix.content[1][0], ModelMatrix.content[1][1], ModelMatrix.content[1][2]).GetLength();
	float z = Vec3D(ModelMatrix.content[2][0], ModelMatrix.content[2][1], ModelMatrix.content[2][2]).GetLength();
	return Vec3D(x, y, z);
}

bool Transform::IsDirty()
{
	return _isDirty;
}

void Transform::SetDirty(bool value)
{
	_isDirty = value;
}

void Transform::ShowComponent()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Vec3D defaultTransform[3] = { _position, _rotation, _scale };
		ImGui::DragFloat3("Position", &_position.x, 0.05f);
		ImGui::DragFloat3("Rotation", &_rotation.x, 0.3f);
		if (!_lockScale) {
			ImGui::DragFloat3("Scale", &_scale.x, 0.01f);
		}
		else {
			ImGui::DragFloat("Scale", &_scale.x, 0.01f);
			_scale = Vec3D(_scale.x);
		}
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &_lockScale);
		if (defaultTransform[0] != _position || defaultTransform[1] != _rotation || defaultTransform[2] != _scale) {
			SetDirty(true);
		}
	}
}

void Transform::Save(std::ofstream& _file, std::string space)
{
#if JSON
	std::string line;
	line += space + string_format("\"Local Position\": [ %f, %f, %f ], \n", this->_position.x, this->_position.y, this->_position.z);
	line += space + string_format("\"Local Rotation\": [ %f, %f, %f ], \n", this->_rotation.x, this->_rotation.y, this->_rotation.z);
	line += space + string_format("\"Local Scale\": [ %f, %f, %f ], \n", this->_scale.x, this->_scale.y, this->_scale.z);
	_file.write(line.c_str(), line.size());
#else
	std::string line;
	line += space + "Local : \n";
	line += space + string_format("	Position { %f, %f, %f } \n", this->_position.x, this->_position.y, this->_position.z);
	line += space + string_format("	Rotation { %f, %f, %f } \n", this->_rotation.x, this->_rotation.y, this->_rotation.z);
	line += space + string_format("	Scale { %f, %f, %f } \n", this->_scale.x, this->_scale.y, this->_scale.z);
	line += space + "World Matrix : ";
	line += ModelMatrix.ToString() + "\n";
	_file.write(line.c_str(), line.size());
#endif
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Transform, m) {
	py::class_<Transform>(m, "Transform")
		.def(py::init<>())
		.def_readwrite("ModelMatrix", &Transform::ModelMatrix)
		.def("GetLocalModelMatrix", &Transform::GetLocalModelMatrix)
		.def("SetLocalTransform", &Transform::SetLocalTransform)
		.def("SetLocalPosition", &Transform::SetLocalPosition)
		.def("SetLocalRotation", &Transform::SetLocalRotation)
		.def("SetLocalScale", &Transform::SetLocalScale)
		.def("SetWorldTransform", &Transform::SetWorldTransform)
		.def("SetWorldPosition", &Transform::SetWorldPosition)
		.def("SetWorldRotation", &Transform::SetWorldRotation)
		.def("SetWorldScale", &Transform::SetWorldScale)
		.def("GetLocalPosition", &Transform::GetLocalPosition)
		.def("GetLocalRotation", &Transform::GetLocalRotation)
		.def("GetLocalScale", &Transform::GetLocalScale)
		.def("GetModelMatrix", &Transform::GetModelMatrix)
		.def("GetWorldPosition", &Transform::GetWorldPosition)
		.def("GetWorldRotation", &Transform::GetWorldRotation)
		.def("GetWorldScale", &Transform::GetWorldScale);
}