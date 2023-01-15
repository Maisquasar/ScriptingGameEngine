#include "../../includes/LowRenderer/Camera.hpp"
#include "../Source/includes/Core/App.hpp"
Camera::Camera() {}
Camera::~Camera() {}

void Camera::FirstUpdate(const float p_deltaTime, bool play)
{
	if (!Script) {
		Script = app.ScriptManager.GetScript("Camera");
		if (!Script)
			return;
	}
	Script->ExecuteFunction("FirstUpdate", this, p_deltaTime, play);
}

void Camera::Update(const float p_deltaTime, bool play)
{
	Script->ExecuteFunction("Update", this, p_deltaTime, play);
}

Mat4D Camera::GetViewMatrix()
{
	Mat4D temp;
	Vec3D z = (position - focus).UnitVector();
	Vec3D x = up.CrossProduct(z).UnitVector();
	Vec3D y = z.CrossProduct(x);
	Vec3D delta = Vec3D(-x.DotProduct(position), -y.DotProduct(position), -z.DotProduct(position));
	for (int i = 0; i < 3; i++)
	{
		temp.at(i, 0) = x[i];
		temp.at(i, 1) = y[i];
		temp.at(i, 2) = z[i];
		temp.at(3, i) = delta[i];
	}
	temp.at(3, 3) = 1;
	return temp;
}

Mat4D Camera::GetProjection()
{
	float s = 1.0f / ((aspect_ratio)*atanf(math::ToRadians(fov / 2.0f)));
	float s2 = 1.0f / atanf(math::ToRadians(fov / 2.0f));
	float param1 = -(Far + Near) / (Far - Near);
	float param2 = -(2 * Near * Far) / (Far - Near);
	Mat4D out;
	out.at(0, 0) = s;
	out.at(1, 1) = s2;
	out.at(2, 2) = param1;
	out.at(2, 3) = -1;
	out.at(3, 2) = param2;
	return out;
}

Mat4D Camera::GetModelMatrix()
{
	return Mat4D::CreateTransformMatrix(position, Vec3D(rotation.y, rotation.x - 180, 0), Vec3D(1));
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Camera, m) {
	py::class_<Camera>(m, "Camera")
		.def_readwrite("position", &Camera::position)
		.def_readwrite("focus", &Camera::focus)
		.def_readwrite("up", &Camera::up)
		.def_readwrite("rotation", &Camera::rotation)
		.def_readwrite("distance", &Camera::distance)
		.def_readwrite("speed", &Camera::speed)
		.def_readwrite("fov", &Camera::fov)
		.def_readwrite("Near", &Camera::Near)
		.def_readwrite("Far", &Camera::Far)
		.def_readwrite("aspect_ratio", &Camera::aspect_ratio)
		.def_readwrite("Lockposition", &Camera::Lockposition)
		.def_readwrite("input", &Camera::input);
}