#pragma once
#include <string>
#include <unordered_map>
#include "../Core/Maths/Maths.hpp"

namespace Loader {

	void SkipLine(const char* data, uint32_t& pos);
	std::string GetString(const char* data, uint32_t& pos, int dec);
	int GetInt(const char* data, uint32_t& pos, int dec);
	float GetFloat(const char* data, uint32_t& pos, int dec);
	Vec4D GetVec4(const char* data, uint32_t& pos, int dec);
	Vec3D GetVec3(const char* data, uint32_t& pos, int dec);
	Vec2D GetVec2(const char* data, uint32_t& pos, int dec);
	Mat4D GetMat4(const char* data, uint32_t& pos, int dec);

	const char* ReadFile(const char* filename, uint32_t& size, bool& success);
}