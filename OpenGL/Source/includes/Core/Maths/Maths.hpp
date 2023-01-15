#pragma once
#include <math.h>
#include <stdbool.h>
#include <filesystem>
#include <iostream>
#include <cstdio>
#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include "../Debug/Log.hpp"
#define PI 3.141592653589793238462643f

#define VEC_COLLINEAR_PRECISION 0.001f
#define VEC_HIGH_VALUE 1e20f

class Vec2D;


class Int2D
{
public:
	int x, y;
	inline Int2D() : x(0), y(0) {}
	inline Int2D(const Int2D& in) : x(in.x), y(in.y) {}
	inline Int2D(const Vec2D& in);
	inline Int2D(const int& a, const int& b) : x(a), y(b) {}

	inline Int2D operator+(const Int2D& a);

	// Return a new vector wich is the substraction of 'a' and 'b'
	inline Int2D operator-(const Int2D& a);

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Int2D operator*(const Int2D& a);

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Int2D operator*(const float& a);

	// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
	inline Int2D operator/(const float& b);

	inline bool operator==(const Int2D& b);
};

class Vec2D
{
public:
	float x;
	float y;

	// Return a new empty Vec2D
	inline Vec2D() : x(0), y(0) {}

	// Return a new Vec2D initialised with 'a' and 'b'
	inline Vec2D(float a, float b) : x(a), y(b) {}

	// Return a new Vec2D initialised with 'in'
	inline Vec2D(const Vec2D& in) : x(in.x), y(in.y) {}
	inline Vec2D(const Int2D& in) : x((float)in.x), y((float)in.y) {}

	// Return the length squared of 'in'
	inline float LengthSquared();

	//Return the distance between 'in' an 'a'
	inline float GetDistanceFromPoint(Vec2D a);

	// Return the lenght of the given Vector
	inline float GetLength();

	// Return a new vector wich is the sum of 'a' and 'b'
	inline Vec2D operator+(const Vec2D& a) const;

	// Return a new vector wich is the substraction of 'a' and 'b'
	inline Vec2D operator-(const Vec2D& a) const;

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Vec2D operator*(const Vec2D& a) const;

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Vec2D operator*(const float& a) const;

	// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
	inline Vec2D operator/(const float& b) const;

	inline bool operator==(const Vec2D& b) const;

	// Return tue if 'a' and 'b' are collinears (Precision defined by VEC2D_COLLINEAR_PRECISION)
	inline bool IsCollinearWith(Vec2D a);

	// Return the dot product of 'a' and 'b'
	inline float DotProduct(Vec2D a);

	// Return the z component of the cross product of 'a' and 'b'
	inline float CrossProduct(Vec2D a);

	// Return a vector with the same direction that 'in', but with length 1
	inline Vec2D UnitVector();

	// Return a vector of length 'in' and with an opposite direction
	inline Vec2D Negate();

	// Return the normal vector of 'in'
	inline Vec2D GetNormal();

	// return true if 'a' converted to int is equivalent to 'in' converted to int
	inline bool IsIntEquivalent(Vec2D a);

	void Print();

	std::string ToString();

};

class Vec3D;
class Vec4D;

class Int3D
{
public:
	int x, y, z;
	inline Int3D() : x(0), y(0), z(0) {}
	inline Int3D(const Int3D& in) : x(in.x), y(in.y), z(in.z) {}
	inline Int3D(const Vec3D& in);
	inline Int3D(const int& a, const int& b, const int& c) : x(a), y(b), z(c) {}

	inline Int3D operator+(const Int3D& a);

	// Return a new vector wich is the substraction of 'a' and 'b'
	inline Int3D operator-(const Int3D& a);

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Int3D operator*(const Int3D& a);

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Int3D operator*(const float& a);

	// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
	inline Int3D operator/(const float& b);

	inline bool operator==(const Int3D& b);

	inline int& operator[](const size_t a);

	void Print();
};

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(float a) : x(a), y(a), z(a), w(a) {}
	Quaternion(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}
	~Quaternion() {}

	Vec3D ToEuler();
	void Print() const;

};

class Vec3D
{
public:
	float x;
	float y;
	float z;

	// Return a new empty Vec3D
	inline Vec3D() : x(0), y(0), z(0) {}

	inline Vec3D(float content) : x(content), y(content), z(content) {}

	inline Vec3D(Vec4D a);

	// Return a new Vec3D initialised with 'a' and 'b'
	inline Vec3D(float a, float b, float c) : x(a), y(b), z(c) {}

	// Return a new Vec3D initialised with 'in'
	inline Vec3D(const Vec3D& in) : x(in.x), y(in.y), z(in.z) {}

	// Print the Vec3D
	void Print() const;

	// Return the length squared of 'in'
	inline float LengthSquared() const;

	// Return the lenght of the given Vector
	inline float GetLength() const;

	inline Vec3D operator=(const Vec4D& b) const;

	inline Vec3D operator=(Vec4D b);

	inline Vec3D operator=(const float b[3]);

	// Return a new vector wich is the sum of 'a' and 'b'
	inline Vec3D operator+(const Vec3D& a) const;

	inline void operator+=(const Vec3D& a);

	inline void operator-=(const Vec3D& a);

	// Return a new vector wich is the substraction of 'a' and 'b'
	inline Vec3D operator-(const Vec3D& a) const;

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Vec3D operator*(const Vec3D& a) const;

	// Return the result of the aritmetic multiplication of 'a' and 'b'
	inline Vec3D operator*(const float& a) const;

	// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC_HIGH_VALUE if 'b' is equal 0;
	inline Vec3D operator/(const float& b) const;

	inline bool operator==(const Vec3D& b) const;

	inline bool operator>(const Vec3D& b) const;

	inline bool operator<(const Vec3D& b) const;

	inline bool operator!=(const Vec3D& b) const;

	inline const float& operator[](const size_t a) const;

	inline float& operator[](const size_t a);

	// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
	inline bool IsCollinearWith(Vec3D a);

	inline float GetDistanceBetween(Vec3D a);

	// Return the dot product of 'a' and 'b'
	inline float DotProduct(Vec3D a);

	// Return the z component of the cross product of 'a' and 'b'
	inline Vec3D CrossProduct(Vec3D a);

	// Return a vector with the same direction that 'in', but with length 1
	inline Vec3D UnitVector() const;

	// Return a vector of length 'in' and with an opposite direction
	inline Vec3D Negate();

	// return true if 'a' converted to int is equivalent to 'in' converted to int
	inline bool IsIntEquivalent(Vec3D a);

	// Convert degree to rad
	inline Vec3D ToRad();

	// Convert rad to Degree
	inline Vec3D ToDeegree();

	inline Vec3D Clamp(Vec3D min, Vec3D max);

	//
	static inline Vec3D ClampMagnitude(Vec3D, float);

	Vec3D Normalize()
	{
		float len = std::sqrt(x * x + y * y + z * z);

		if (len != 0.)
		{

			return Vec3D(x / len, y / len, z / len);
		}
		return Vec3D(x, y, z);
	}

	inline Quaternion ToQuaternion()
	{
		// Abbreviations for the various angular functions
		float cy = cosf(z * 0.5f);
		float sy = sinf(z * 0.5f);
		float cp = cosf(y * 0.5f);
		float sp = sinf(y * 0.5f);
		float cr = cosf(x * 0.5f);
		float sr = sinf(x * 0.5f);

		Quaternion q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	}

	std::string ToString();
};

class UChar4D
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	inline UChar4D() : r(0), g(0), b(0), a(0) {}
	inline UChar4D(const float* in);
	inline UChar4D(const Vec4D& in);
	inline UChar4D(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xff) : r(red), g(green), b(blue), a(alpha) {}
	inline UChar4D(unsigned int rgba) : r((rgba & 0xff000000) >> 24), g((rgba & 0x00ff0000) >> 16), b((rgba & 0x0000ff00) >> 8), a(rgba & 0x000000ff) {}

	inline UChar4D operator*(const float& a);
	inline UChar4D operator+(const UChar4D& a);
};

class Vec4D
{
public:
	float x;
	float y;
	float z;
	float w;

	// Return a new empty Vec4D
	inline Vec4D() : x(0), y(0), z(0), w(0) {}

	inline Vec4D(float a) : x(a), y(a), z(a), w(a) {}

	// Return a new Vec4D initialised with 'a', 'b', 'c' and 'd'
	inline Vec4D(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

	// Return a new Vec4D initialised with 'in'
	inline Vec4D(const Vec3D& in, float wIn = 1.0f) : x(in.x), y(in.y), z(in.z), w(wIn) {}

	// Return a new Vec4D initialised with 'in'
	inline Vec4D(const Vec4D& in) : x(in.x), y(in.y), z(in.z), w(in.w) {}

	inline Vec4D(const UChar4D& in) : x(in.r / 255.0f), y(in.g / 255.0f), z(in.b / 255.0f), w(in.a / 255.0f) {}


	// Print the Vec4D
	void Print();

	std::string ToString();

	// Return the Vec3D of Vec4D
	inline Vec3D GetVector() const;

	// Return the length squared
	inline float LengthSquared() const;

	// Return the length
	inline float GetLength() const;

	// Divide each components by w, or set to VEC_HIGH_VALUE if w equals 0
	inline Vec4D Homogenize() const;

	inline Vec4D operator+(const Vec4D& a) const;

	inline Vec4D operator-(const Vec4D& a) const;

	inline Vec4D operator*(const Vec4D& a) const;

	inline Vec4D operator*(const float& a) const;

	inline Vec4D operator/(const float& b) const;

	inline bool operator==(const Vec4D& b);

	inline float& operator[](const size_t a);
	inline const float& operator[](const size_t a) const;

	// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
	inline bool IsCollinearWith(Vec4D a);

	inline float DotProduct(Vec4D a);

	// Return the z component of the cross product of 'a' and 'b'
	inline Vec4D CrossProduct(Vec4D a);

	// Return a vector with the same direction that 'in', but with length 1
	inline Vec4D UnitVector();

	// Return a vector of length 'in' and with an opposite direction
	inline Vec4D Negate();

	// return true if 'a' converted to int is equivalent to 'in' converted to int
	inline bool IsIntEquivalent(Vec4D a);
};


class Mat3D
{
public:
	/* data of the matrix : content[y][x]
	 * Matrix is indexed with:
	 *
	 * 00 | 01 | 02
	 * 10 | 11 | 12
	 * 20 | 21 | 22

	 *
	*/
	float content[3][3] = { 0 };

	Mat3D() {}

	Mat3D(float diagonal);

	Mat3D(const Mat3D& in);

	Mat3D(const float* data);

	Mat3D operator*(const Mat3D& a);

	Vec3D operator*(const Vec3D& a);

	static Mat3D Identity();

	Mat3D CreateInverseMatrix();

	Mat3D CreateAdjMatrix();

	Mat3D GetCofactor(int p, int q, int n);

	// Recursive function for finding determinant of matrix. n is current dimension of 'in'.
	int GetDeterminant(int n);

	Mat3D TransposeMatrix();

	inline float& operator[](const size_t a);

	inline float& at(const unsigned char x, const unsigned char y);

	void PrintMatrix();

};

class Mat4D
{
public:
	/* data of the matrix : content[y][x]
	 * Matrix is indexed with:
	 *
	 * 00 | 01 | 02 | 03
	 * 10 | 11 | 12 | 13
	 * 20 | 21 | 22 | 23
	 * 30 | 31 | 32 | 33
	 *
	*/
	float content[4][4] = { 0 };

	// Return a new empty Vec4D
	Mat4D() {}

	Mat4D(float diagonal);

	Mat4D(const Mat4D& in);

	Mat4D(const float* data);

	Mat4D Multiply(Mat4D a) const;

	Mat4D operator*(const Mat4D& a) const;

	Vec4D operator*(const Vec4D& a) const;

	Mat4D operator+(const Mat4D& a) const;

	static Mat4D Identity();

	static Mat4D CreateTransformMatrix(const Vec3D& position, const Vec3D& rotation, const Vec3D& scale);

	static Mat4D CreateTranslationMatrix(const Vec3D& translation);

	static Mat4D CreateScaleMatrix(const Vec3D& scale);

	static Mat4D CreateXRotationMatrix(float angle);

	static Mat4D CreateYRotationMatrix(float angle);

	static Mat4D CreateZRotationMatrix(float angle);

	static Mat4D CreateRotationMatrix(const Vec3D& rotation);

	static Mat4D CreatePerspectiveProjectionMatrix(float, float, float);

	Vec3D GetPosition();
	Vec3D GetRotation();
	Vec3D GetScale();

	Mat4D CreateInverseMatrix();

	Mat4D CreateAdjMatrix();

	Mat4D GetCofactor(int p, int q, int n);

	// Recursive function for finding determinant of matrix. n is current dimension of 'in'.
	int GetDeterminant(int n);

	Mat4D TransposeMatrix();

	inline float& operator[](const size_t a);

	inline float& at(const unsigned char x, const unsigned char y);

	void PrintMatrix();

	std::string ToString()
	{
		std::string print;
		for (int j = 0; j < 4; j++)
		{
			print += "{";
			for (int i = 0; i < 4; i++)
			{
				print += " ";
				print += string_format("%.2f", content[j][i]);
			}
			print += "}";
		}
		return print;
	}

};


struct Viewport;

namespace math
{
	// Return the given angular value in degrees converted to radians
	inline float ToRadians(float in);

	// Return the given angular value in radians converted to degrees
	inline float ToDegrees(float in);

	inline float Cut(float in, float min, float max);

	inline int CutInt(int in, int min, int max);

	inline float Mod(float in, float value);

	inline float Min(float a, float b);

	inline float Max(float a, float b);

	inline bool IsEqual(float a, float b, float prec);

	inline bool IsEqualVec4(Vec4D a, Vec4D b, float prec);

	inline float EdgeFunction(const Int2D p, const Int2D a, const Int2D b);

	inline bool IsTopLeft(const Int2D& a, const Int2D& b);

	static void PrintMatrix(Mat4D a);

	inline Vec3D GetSphericalCoords(float r, float theta, float phi);

	inline std::vector<float> GetCubeVertices(std::vector<Vec3D>& v, std::vector<Vec3D>& n);

	inline std::vector<float> GetSphereVertices(std::vector<Vec3D>& v, std::vector<Vec3D>& n, float radius);

	inline std::vector<float> GetDemiSphereVertices(float radius, bool up = true);

	inline std::vector<float> GetCylindreVertices(float radius, float height);

	inline float GetMaxX(std::vector<Vec3D> points);
	inline float GetMaxY(std::vector<Vec3D> points);
	inline float GetMaxZ(std::vector<Vec3D> points);

	inline float GetMinX(std::vector<Vec3D> points);
	inline float GetMinY(std::vector<Vec3D> points);
	inline float GetMinZ(std::vector<Vec3D> points);

};

static int StringToInt(std::string text)
{
	int result = 0;
	int sign = 1;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == '-')
			sign = -1;
		if (text[i] >= '0' && text[i] <= '9') {
			result *= 10;
			result += text[i] - '0';
		}
	}
	return result * sign;
}

static float StringToFloat(std::string text)
{
	float result = 0;
	bool dot = false;
	int sign = 1;
	int divide = 1;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] >= '0' && text[i] <= '9' && !dot) {
			result *= 10;
			result += text[i] - '0';
		}
		else if (text[i] >= '0' && text[i] <= '9')
		{
			divide *= 10;
			result += (float)(text[i] - '0') / divide;
		}
		if (text[i] == '.')
			dot = true;
		if (text[i] == '-')
			sign = -1;
	}
	return result * sign;
}

#include "maths.inl"