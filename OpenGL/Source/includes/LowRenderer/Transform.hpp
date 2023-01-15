#pragma once
#include "../Core/Maths/Maths.hpp"
#include <pybind11/pybind11.h>

class Transform
{
protected:
	//Local space information
	Vec3D _position = { 0.0f, 0.0f, 0.0f };
	Vec3D _rotation = { 0.0f, 0.0f, 0.0f }; //In degrees
	Vec3D _scale = { 1.0f, 1.0f, 1.0f };

	//Global space information concatenate in matrix

	//Dirty flag
	bool _lockScale = false;
	bool _isDirty = true;

public:
	Transform() { }

	Mat4D ModelMatrix = Mat4D::Identity();

	Mat4D GetLocalModelMatrix();

	void ComputeModelMatrix();

	void ComputeModelMatrix(Mat4D parentGlobalModelMatrix);

	// Setters for Local.
	void SetLocalTransform(const Vec3D& newPosition, const Vec3D& newRotation, const Vec3D& newScale);
	void SetLocalPosition(const Vec3D& newPosition);
	void SetLocalRotation(const Vec3D& newRotation);
	void SetLocalScale(const Vec3D& newScale);

	// Setters for World.
	void SetWorldTransform(const Vec3D& newPosition, const Vec3D& newRotation, const Vec3D& newScale = Vec3D(0));
	void SetWorldPosition(const Vec3D& newPosition);
	void SetWorldRotation(const Vec3D& newRotation);
	void SetWorldScale(const Vec3D& newScale);

	// Getters local.
	const Vec3D& GetLocalPosition();
	const Vec3D& GetLocalRotation();
	const Vec3D& GetLocalScale();
	// Getters World.
	const Mat4D& GetModelMatrix();
	const Vec3D GetWorldPosition();
	const Vec3D GetWorldRotation();
	const Vec3D GetWorldScale();
	
	bool IsDirty();

	void SetDirty(bool value);

	void ShowComponent();

	// Save.
	void Save(std::ofstream& _file, std::string space);
};
