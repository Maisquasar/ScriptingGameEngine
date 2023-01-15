#pragma once
#include "../Core/Maths/Maths.hpp"
#include "../Core/Input.hpp"
#include "../LowRenderer/Transform.hpp"
#include "../Resources/Components/CameraLock.hpp"
#include <pybind11/pybind11.h>
class Mat4D;

class Camera
{
public:
    Vec3D position;
    Vec3D focus;
    Vec3D up = Vec3D(0, 1, 0);
    Vec2D rotation = Vec2D(-PI/2, -PI/2);
    float distance = 5;
    float fov = 70;
    float Near = 0.1f;
    float Far = 1000.f;
    float aspect_ratio = (4.0f / 3.0f);
    float speed = 25.f;

    CameraLock* Lockposition = nullptr;
    Input input;

    PythonScript* Script;

    Camera();
    ~Camera();

    void FirstUpdate(const float p_deltaTime, bool play);
    void Update(const float p_deltaTime, bool play);
    // Return the Mat4D view matrix from the camera.
    Mat4D GetViewMatrix();
    // Return the Mat4D projection matrix from the camera.
    Mat4D GetProjection();
    // Return the Mat4D model matrix.
    Mat4D GetModelMatrix();

    
};
