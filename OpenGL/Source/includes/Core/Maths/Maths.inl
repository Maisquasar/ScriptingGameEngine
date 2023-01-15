#include <cstdio>

#include "maths.hpp"

// -----------------------   Int2D    -----------------------

inline Int2D::Int2D(const Vec2D& in) : x((int)in.x), y((int)in.y) {}

inline Int2D Int2D::operator+(const Int2D& a)
{
	Int2D res = Int2D(a.x + this->x, a.y + this->y);
	return res;
}

inline Int2D Int2D::operator-(const Int2D& a)
{
	return Int2D(x - a.x, y - a.y);
}

inline Int2D Int2D::operator*(const Int2D& a)
{
	Int2D res = Int2D(this->x * a.x, this->y * a.y);
	return res;
}

inline Int2D Int2D::operator*(const float& a)
{
	Int2D res = Int2D(this->x * (int)a, this->y * (int)a);
	return res;
}

inline Int2D Int2D::operator/(const float& a)
{
	if (a == 0)
		return Int2D(INT32_MAX, INT32_MAX);
	Int2D res = Int2D(x / (int)a, y / (int)a);
	return res;
}

inline bool Int2D::operator==(const Int2D& b)
{
	return (x == b.x && y == b.y);
}

// -----------------------   Vec2D    -----------------------

inline float Vec2D::LengthSquared()
{
	return (x * x + y * y);
}

inline float Vec2D::GetLength()
{
	return sqrtf(LengthSquared());
}

inline Vec2D Vec2D::operator+(const Vec2D& a) const
{
	Vec2D res = Vec2D(a.x + this->x, a.y + this->y);
	return res;
}

inline Vec2D Vec2D::operator-(const Vec2D& a) const
{
	Vec2D res = Vec2D(this->x - a.x, this->y - a.y);
	return res;
}

inline Vec2D Vec2D::operator*(const Vec2D& a)const
{
	Vec2D res = Vec2D(this->x * a.x, this->y * a.y);
	return res;
}

inline Vec2D Vec2D::operator*(const float& a)const
{
	Vec2D res = Vec2D(this->x * a, this->y * a);
	return res;
}

inline Vec2D Vec2D::operator/(const float& a)const
{
	if (a == 0.0)
		return operator*(VEC_HIGH_VALUE);
	Vec2D res = operator*(1 / a);
	return res;
}

inline bool Vec2D::operator==(const Vec2D& b)const
{
	return (x == b.x && y == b.y);
}

inline bool Vec2D::IsCollinearWith(Vec2D a)
{
	float res = a.x * y - a.y * x;
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vec2D::DotProduct(Vec2D a)
{
	return (a.x * x + a.y * y);
}

inline float Vec2D::CrossProduct(Vec2D a)
{
	return (x * a.y - y * a.x);
}

inline Vec2D Vec2D::UnitVector()
{
	return operator/(GetLength());
}

inline Vec2D Vec2D::Negate()
{
	return operator*(-1);
}

inline Vec2D Vec2D::GetNormal()
{
	return Vec2D(-y, x);
}

inline bool Vec2D::IsIntEquivalent(Vec2D a)
{
	return ((int)x == (int)a.x && (int)y == a.y);
}

inline float Vec2D::GetDistanceFromPoint(Vec2D a)
{
	float i = a.x - x;
	float j = a.y - y;
	return sqrt(i * i + j * j);
}

// -----------------------   Int3D    -----------------------

inline Int3D::Int3D(const Vec3D& in) : x((int)in.x), y((int)in.y), z((int)in.z) {}

inline Int3D Int3D::operator+(const Int3D& a)
{
	return Int3D(a.x + this->x, a.y + this->y, a.z + this->z);
}

inline Int3D Int3D::operator-(const Int3D& a)
{
	return Int3D(x - a.x, y - a.y, z - a.z);
}

inline Int3D Int3D::operator*(const Int3D& a)
{
	Int3D res = Int3D(this->x * a.x, this->y * a.y, this->z * a.z);
	return res;
}

inline Int3D Int3D::operator*(const float& a)
{
	Int3D res = Int3D(this->x * (int)a, this->y * (int)a, this->z * (int)a);
	return res;
}

inline Int3D Int3D::operator/(const float& a)
{
	if (a == 0)
		return Int3D(INT32_MAX, INT32_MAX, INT32_MAX);
	Int3D res = Int3D(x / (int)a, y / (int)a, z / (int)a);
	return res;
}

inline bool Int3D::operator==(const Int3D& b)
{
	return (x == b.x && y == b.y && z == b.z);
}

inline int& Int3D::operator[](const size_t a)
{
	return *((&x) + a);
}

// -----------------------   Vec3D    -----------------------

inline Vec3D::Vec3D(Vec4D a) : x(a.x), y(a.y), z(a.z) {}

inline float Vec3D::LengthSquared() const
{
	return (x * x + y * y + z * z);
}

inline float Vec3D::GetLength() const
{
	return sqrtf(LengthSquared());
}

inline Vec3D Vec3D::operator=(const Vec4D& b) const
{
	return Vec3D(b.x, b.y, b.z);
}
inline Vec3D Vec3D::operator=(Vec4D b)
{
	return Vec3D(b.x, b.y, b.z);
}
inline Vec3D Vec3D::operator=(const float b[3])
{
	return Vec3D(b[0], b[1], b[2]);
}

inline Vec3D Vec3D::operator+(const Vec3D& a) const
{
	Vec3D res = Vec3D(a.x + this->x, a.y + this->y, a.z + this->z);
	return res;
}


inline void Vec3D::operator+=(const Vec3D& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
}

inline void Vec3D::operator-=(const Vec3D& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
}

inline Vec3D Vec3D::operator-(const Vec3D& a) const
{
	Vec3D res = Vec3D(this->x - a.x, this->y - a.y, this->z - a.z);
	return res;
}

inline Vec3D Vec3D::operator*(const Vec3D& a) const
{
	Vec3D res = Vec3D(this->x * a.x, this->y * a.y, this->z * a.z);
	return res;
}

inline Vec3D Vec3D::operator*(const float& a) const
{
	Vec3D res = Vec3D(this->x * a, this->y * a, this->z * a);
	return res;
}

inline Vec3D Vec3D::operator/(const float& a) const
{
	if (a == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vec3D res = operator*(1 / a);
	return res;
}

inline bool Vec3D::operator==(const Vec3D& b) const
{
	return (x == b.x && y == b.y && z == b.z);
}

inline bool Vec3D::operator>(const Vec3D& b) const
{
	return (x > b.x || y > b.y || z > b.z);
}

inline bool Vec3D::operator<(const Vec3D& b) const
{
	return (x < b.x || y < b.y || z < b.z);
}

inline bool Vec3D::operator!=(const Vec3D& b) const
{
	return (x != b.x || y != b.y || z != b.z);
}

inline float& Vec3D::operator[](const size_t a)
{
	return *((&x) + a);
}

inline const float& Vec3D::operator[](const size_t a) const
{
	return *((&x) + a);
}

inline float Vec3D::DotProduct(Vec3D a)
{
	return (a.x * x + a.y * y + a.z * z);
}

inline bool Vec3D::IsCollinearWith(Vec3D a)
{
	float res = this->DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vec3D::GetDistanceBetween(Vec3D a)
{
	float i = a.x - x;
	float j = a.y - y;
	float h = a.z - z;
	return sqrt(i * i + j * j + h * h);
}


inline Vec3D Vec3D::CrossProduct(Vec3D a)
{
	return Vec3D((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x));
}

inline Vec3D Vec3D::UnitVector() const
{
	return operator/(GetLength());
}

inline Vec3D Vec3D::Negate()
{
	return operator*(-1);
}

inline bool Vec3D::IsIntEquivalent(Vec3D a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z);
}


// Convert degree to rad
inline Vec3D Vec3D::ToRad()
{
	return Vec3D(math::ToRadians(x), math::ToRadians(y), math::ToRadians(z));
}

// Convert rad to Degree
inline Vec3D Vec3D::ToDeegree()
{
	return Vec3D(math::ToDegrees(x), math::ToDegrees(y), math::ToDegrees(z));
}

inline Vec3D Vec3D::Clamp(Vec3D min, Vec3D max)
{
	Vec3D tmp = *this;
	if (tmp < min)
		tmp = min;
	else if (tmp > max)
		tmp = max;
	return tmp;
}

inline Vec3D Vec3D::ClampMagnitude(Vec3D vector, float maxLength)
{
	float sqrmag = vector.LengthSquared();
	if (sqrmag > maxLength * maxLength)
	{
		float mag = (float)sqrtf(sqrmag);
		float normalized_x = vector.x / mag;
		float normalized_y = vector.y / mag;
		float normalized_z = vector.z / mag;
		return Vec3D(normalized_x * maxLength,
			normalized_y * maxLength,
			normalized_z * maxLength);
	}
	return vector;
}
// -----------------------   Vec4D    -----------------------

inline Vec3D Vec4D::GetVector() const
{
	return Vec3D(x, y, z);
}

inline Vec4D Vec4D::Homogenize() const
{
	return Vec4D(GetVector() / w);
}

inline Vec4D Vec4D::UnitVector()
{
	Vec4D homogenized = Homogenize();
	return homogenized / homogenized.GetVector().GetLength();
}

inline float Vec4D::LengthSquared() const
{
	return Homogenize().GetVector().LengthSquared();
}

inline float Vec4D::GetLength() const
{
	return sqrtf(LengthSquared());
}

inline Vec4D Vec4D::operator+(const Vec4D& a) const
{
	return Vec4D(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline Vec4D Vec4D::operator-(const Vec4D& a) const
{
	return Vec4D(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline Vec4D Vec4D::operator*(const Vec4D& a) const
{
	return Vec4D(x * a.x, y * a.y, z * a.z, w * a.w);
}

inline Vec4D Vec4D::operator*(const float& a) const
{
	return Vec4D(this->x * a, this->y * a, this->z * a, this->w * a);
}
inline Vec4D Vec4D::operator/(const float& b) const
{
	if (b == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vec4D res = operator*(1 / b);
	return res;
}

inline bool Vec4D::operator==(const Vec4D& b)
{
	return (x == b.x && y == b.y && z == b.z && w == b.w);
}

inline float& Vec4D::operator[](const size_t a)
{
	return *((&x) + a);
}

inline const float& Vec4D::operator[](const size_t a) const
{
	return *((&x) + a);
}

inline bool Vec4D::IsCollinearWith(Vec4D a)
{
	float res = DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vec4D::DotProduct(Vec4D a)
{
	return (a.x * x + a.y * y + a.z * z);
}

inline Vec4D Vec4D::CrossProduct(Vec4D a)
{
	return Vec4D((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
}

inline Vec4D Vec4D::Negate()
{
	return operator*(-1);
}

inline bool Vec4D::IsIntEquivalent(Vec4D a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z && (int)w == (int)a.w);
}

// -----------------------   UChar4D    -----------------------

inline UChar4D::UChar4D(const float* in)
{
	r = (unsigned char)(in[0] * 255);
	g = (unsigned char)(in[1] * 255);
	b = (unsigned char)(in[2] * 255);
	a = (unsigned char)(in[3] * 255);
}

inline UChar4D::UChar4D(const Vec4D& in)
{
	r = (unsigned char)(math::Cut(in[0], 0, 1) * 255);
	g = (unsigned char)(math::Cut(in[1], 0, 1) * 255);
	b = (unsigned char)(math::Cut(in[2], 0, 1) * 255);
	a = (unsigned char)(math::Cut(in[3], 0, 1) * 255);
}

inline UChar4D UChar4D::operator*(const float& in)
{
	return UChar4D(r * (unsigned char)in, g * (unsigned char)in, b * (unsigned char)in, a);
}

inline UChar4D UChar4D::operator+(const UChar4D& in)
{
	return UChar4D(r + in.r, g + in.g, b + in.b, a);
}

// -----------------------   Mat3D    -----------------------

inline float& Mat3D::operator[](const size_t in)
{
	return content[in / 3][in % 3];
}

inline float& Mat3D::at(const unsigned char x, const unsigned char y)
{
	if (x >= 3 || y >= 3)
		return content[0][0];
	else
		return content[y][x];
}
// -----------------------   Mat4D    -----------------------

inline float& Mat4D::operator[](const size_t in)
{
	return content[in / 4][in % 4];
}

inline float& Mat4D::at(const unsigned char x, const unsigned char y)
{
	if (x > 3 || y > 3)
		return content[0][0];
	return content[y][x];
}

// ----------------------- Math Utils -----------------------

inline float math::ToRadians(float in)
{
	return in / 180.f * PI;
}

inline float math::ToDegrees(float in)
{
	return in * 180.f / PI;
}

inline float math::Cut(float in, float min, float max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

inline int math::CutInt(int in, int min, int max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

inline float math::Mod(float in, float value)
{
	return in - value * floorf(in / value);
}

inline float math::Min(float a, float b)
{
	if (a > b)
		return b;
	return a;
}

inline float math::Max(float a, float b)
{
	if (a > b)
		return a;
	return b;
}

inline bool math::IsEqual(float a, float b, float prec)
{
	return (a - prec < b&& a + prec > b);
}

inline bool math::IsEqualVec4(Vec4D a, Vec4D b, float prec)
{
	return (IsEqual(b.x, a.x, prec) && IsEqual(b.y, a.y, prec) && IsEqual(b.z, a.z, prec) && IsEqual(b.w, a.w, prec));
}

inline float math::EdgeFunction(const Int2D p, const Int2D a, const Int2D b)
{
	return (float)(p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

inline bool math::IsTopLeft(const Int2D& a, const Int2D& b)
{
	return (a.y > b.y || (a.y == b.y && a.x < b.x));
}

inline Vec3D math::GetSphericalCoords(float r, float theta, float phi)
{
	return { r * sinf(theta) * cosf(phi),
			r * cosf(theta),
			r * sinf(theta) * sinf(phi) };
}

inline std::vector<float> math::GetCubeVertices(std::vector<Vec3D>& v, std::vector<Vec3D>& n)
{
	std::vector<float> vOut;
	v = {
		{ 1.000000, 1.000000, -1.000000},
		{ 1.000000, -1.000000, -1.000000},
		{ 1.000000, 1.000000, 1.000000},
		{ 1.000000, -1.000000, 1.000000},
		{ -1.000000, 1.000000, -1.000000},
		{ -1.000000, -1.000000, -1.000000},
		{ -1.000000, 1.000000, 1.000000},
		{ -1.000000, -1.000000, 1.000000},
	};
	n = {
		{0.0000, 1.0000, 0.0000},
		{0.0000, 0.0000, 1.0000},
		{-1.0000, 0.0000, 0.0000},
		{0.0000, -1.0000, 0.0000},
		{1.0000, 0.0000, 0.0000},
		{0.0000, 0.0000, -1.0000},
	};

	std::vector<int> indices{
	5, 1, 3, 1, 1 ,1,
	3, 2, 8, 2, 4 ,2,
	7, 3, 6, 3, 8 ,3,
	2, 4, 8, 4, 6 ,4,
	1, 5, 4, 5, 2 ,5,
	5, 6, 2, 6, 6 ,6,
	5, 1, 7, 1, 3 ,1,
	3, 2, 7, 2, 8 ,2,
	7, 3, 5, 3, 6 ,3,
	2, 4, 4, 4, 8 ,4,
	1, 5, 3, 5, 4 ,5,
	5, 6, 1, 6, 2 ,6,
	};

	for (int k = 0; k < indices.size(); k += 2)
	{
		indices[k] -= 1;
		indices[k + 1ll] -= 1;
		vOut.push_back(v[indices[k]].x);
		vOut.push_back(v[indices[k]].y);
		vOut.push_back(v[indices[k]].z);

		vOut.push_back(n[indices[k + 1ll]].x);
		vOut.push_back(n[indices[k + 1ll]].y);
		vOut.push_back(n[indices[k + 1ll]].z);
	}

	return vOut;
}

inline std::vector<float> math::GetSphereVertices(std::vector<Vec3D>& v, std::vector<Vec3D>& n, float radius)
{
	std::vector<float> vOut;
	float r = radius;
	int mLat = 20;
	int mLon = 20;
	for (int j = 0; j < mLat; ++j)
	{
		float theta0 = ((j + 0) / (float)mLat) * PI;
		float theta1 = ((j + 1) / (float)mLat) * PI;

		for (int i = 0; i < mLon; ++i)
		{
			float phi0 = ((i + 0) / (float)mLon) * 2.f * PI;
			float phi1 = ((i + 1) / (float)mLon) * 2.f * PI;

			Vec3D c0 = math::GetSphericalCoords(r, theta0, phi0);
			Vec3D c1 = math::GetSphericalCoords(r, theta0, phi1);
			Vec3D c2 = math::GetSphericalCoords(r, theta1, phi1);
			Vec3D c3 = math::GetSphericalCoords(r, theta1, phi0);

			v.push_back(c0);
			v.push_back(c1);
			v.push_back(c2);

			v.push_back(c0);
			v.push_back(c2);
			v.push_back(c3);

			n.push_back(c0.UnitVector());
			n.push_back(c1.UnitVector());
			n.push_back(c2.UnitVector());

			n.push_back(c0.UnitVector());
			n.push_back(c2.UnitVector());
			n.push_back(c3.UnitVector());
		}
	}

	for (int k = 0; k < v.size(); k++)
	{
		vOut.push_back(v[k].x);
		vOut.push_back(v[k].y);
		vOut.push_back(v[k].z);

		vOut.push_back(n[k].x);
		vOut.push_back(n[k].y);
		vOut.push_back(n[k].z);
	}

	return vOut;
}

inline std::vector<float> math::GetDemiSphereVertices(float radius, bool up)
{
	std::vector<Vec3D> v;
	std::vector<Vec3D> n;
	std::vector<float> vOut;
	float r = radius;
	int mLat = 10;
	int mLon = 20;

	for (int j = 0; j < mLat; ++j)
	{
		float theta0;
		float theta1;
		if (!up)
		{
			theta0 = ((j + 0) / (float)mLat) * PI + PI / 2;
			theta1 = ((j + 1) / (float)mLat) * PI + PI / 2;
		}
		else
		{
			theta0 = ((j + 0) / (float)mLat) * PI / 2;
			theta1 = ((j + 1) / (float)mLat) * PI / 2;
		}

		for (int i = 0; i < mLon; ++i)
		{
			float phi0 = ((i + 0) / (float)mLon) * 2.f * PI;
			float phi1 = ((i + 1) / (float)mLon) * 2.f * PI;

			Vec3D c0 = math::GetSphericalCoords(r, theta0, phi0);
			Vec3D c1 = math::GetSphericalCoords(r, theta0, phi1);
			Vec3D c2 = math::GetSphericalCoords(r, theta1, phi1);
			Vec3D c3 = math::GetSphericalCoords(r, theta1, phi0);

			v.push_back(c0);
			v.push_back(c1);
			v.push_back(c2);

			v.push_back(c0);
			v.push_back(c2);
			v.push_back(c3);

			n.push_back(c0.UnitVector());
			n.push_back(c1.UnitVector());
			n.push_back(c2.UnitVector());

			n.push_back(c0.UnitVector());
			n.push_back(c2.UnitVector());
			n.push_back(c3.UnitVector());
		}
	}

	for (int k = 0; k < v.size(); k++)
	{
		vOut.push_back(v[k].x);
		vOut.push_back(v[k].y);
		vOut.push_back(v[k].z);

		vOut.push_back(n[k].x);
		vOut.push_back(n[k].y);
		vOut.push_back(n[k].z);
	}

	return vOut;
}

inline std::vector<float> math::GetCylindreVertices(float radius, float height)
{
	int res = 20;
	std::vector<Vec3D> v;
	std::vector<Vec3D> n;
	std::vector<float> vOut;
	float r = radius;
	int mLon = 20;

	float theta0 = ((10 + 0) / (float)20) * PI;
	float theta1 = ((10 + 1) / (float)20) * PI;
	for (int i = 0; i < mLon; ++i)
	{
		float phi0 = ((i + 0) / (float)mLon) * 2.f * PI;
		float phi1 = ((i + 1) / (float)mLon) * 2.f * PI;

		Vec3D c0 = math::GetSphericalCoords(r, theta0, phi0);
		Vec3D c1 = math::GetSphericalCoords(r, theta0, phi1);
		Vec3D c2 = math::GetSphericalCoords(r, theta1, phi1);
		Vec3D c3 = math::GetSphericalCoords(r, theta1, phi0);

		c0.y += (height / 2);
		c1.y += (height / 2);
		c2.y -= (height / 2);
		c3.y -= (height / 2);

		v.push_back(c0);
		v.push_back(c1);
		v.push_back(c2);

		v.push_back(c0);
		v.push_back(c2);
		v.push_back(c3);

		n.push_back(c0.UnitVector());
		n.push_back(c1.UnitVector());
		n.push_back(c2.UnitVector());

		n.push_back(c0.UnitVector());
		n.push_back(c2.UnitVector());
		n.push_back(c3.UnitVector());
	}

	for (int k = 0; k < v.size(); k++)
	{
		vOut.push_back(v[k].x);
		vOut.push_back(v[k].y);
		vOut.push_back(v[k].z);

		vOut.push_back(n[k].x);
		vOut.push_back(n[k].y);
		vOut.push_back(n[k].z);
	}

	return vOut;
}


inline float math::GetMaxX(std::vector<Vec3D> points)
{
	float x = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || x < points[i].x)
			x = points[i].x;
	}
	return x;
}
inline float math::GetMaxY(std::vector<Vec3D> points) {

	float y = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || y < points[i].y)
			y = points[i].y;
	}
	return y;
}
inline float math::GetMaxZ(std::vector<Vec3D> points) {
	float z = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || z < points[i].z)
			z = points[i].z;
	}
	return z;
}

inline float math::GetMinX(std::vector<Vec3D> points) {
	float x = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || x > points[i].x)
			x = points[i].x;
	}
	return x;
}
inline float math::GetMinY(std::vector<Vec3D> points) {
	float y = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || y > points[i].y)
			y = points[i].y;
	}
	return y;

}
inline float math::GetMinZ(std::vector<Vec3D> points) {
	float z = -1;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (i == 0 || z > points[i].z)
			z = points[i].z;
	}
	return z;
}