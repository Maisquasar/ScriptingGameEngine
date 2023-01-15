#include <cstdio>
#include "../Source/includes/Core/Maths/Maths.hpp"

// -----------------------   Vec2D    -----------------------
void Vec2D::Print()
{
	printf("(%.2f, %.2f)\n", x, y);
}

std::string Vec2D::ToString()
{
	return std::to_string(x) + " " + std::to_string(y);
}
// -----------------------   Int3D    -----------------------
void Int3D::Print()
{
	printf("(%d, %d, %d)\n", x, y, z);
}
// -----------------------   Vec3D    -----------------------

void Vec3D::Print() const
{
	printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}
std::string Vec3D::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

// -----------------------   Vec4D    -----------------------

void Vec4D::Print()
{
	printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

std::string Vec4D::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}

// -----------------------   Mat3D    -----------------------

Mat3D::Mat3D(float diagonal)
{
	for (size_t i = 0; i < 3; i++) content[i][i] = diagonal;
}

Mat3D::Mat3D(const Mat3D& in)
{
	for (size_t j = 0; j < 3; j++)
	{
		for (size_t i = 0; i < 3; i++)
		{
			content[j][i] = in.content[j][i];
		}
	}
}

Mat3D::Mat3D(const float* data)
{
	for (size_t j = 0; j < 3; j++)
	{
		for (size_t i = 0; i < 3; i++)
		{
			content[j][i] = data[j * 3 + i];
		}
	}
}

Mat3D Mat3D::operator*(const Mat3D& in)
{
	Mat3D out;
	for (size_t j = 0; j < 3; j++)
	{
		for (size_t i = 0; i < 3; i++)
		{
			float res = 0;
			for (size_t k = 0; k < 3; k++)
				res += content[j][k] * in.content[k][i];

			out.content[j][i] = res;
		}
	}
	return out;
}

Vec3D Mat3D::operator*(const Vec3D& in)
{
	Vec3D out;
	for (size_t i = 0; i < 3; i++)
	{
		float res = 0;
		for (size_t k = 0; k < 3; k++) res += content[i][k] * in[k];
		out[i] = res;
	}
	return out;
}

Mat3D Mat3D::GetCofactor(int p, int q, int n)
{
	Mat3D mat;
	int i = 0, j = 0;
	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element
			//  which are not in given row and column
			if (row != p && col != q)
			{
				mat.content[i][j++] = content[row][col];

				// Row is filled, so increase row index and
				// reset col index
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
	return mat;
}

int Mat3D::GetDeterminant(int n)
{
	Mat3D a;
	int D = 0; // Initialize result

	//  Base case : if matrix contains single element
	if (n == 1)
		return (int)content[0][0];

	int sign = 1;  // To store sign multiplier

	 // Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of matrix[0][f]
		a = GetCofactor(0, f, n);
		D += sign * (int)content[0][f] * a.GetDeterminant(n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}

Mat3D Mat3D::CreateInverseMatrix()
{
	// Find determinant of matrix
	Mat3D inverse;
	int det = GetDeterminant(4);
	if (det == 0)
	{
		printf("Singular matrix, can't find its inverse\n");
		return 1;
	}

	// Find adjoint
	Mat3D adj = CreateAdjMatrix();

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			inverse.content[i][j] = adj.content[i][j] / float(det);

	return inverse;
}

Mat3D Mat3D::CreateAdjMatrix()
{
	// temp is used to store cofactors of matrix
	Mat3D temp;
	Mat3D adj;
	int sign = 1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// Get cofactor of matrix[i][j]
			temp = GetCofactor(i, j, 4);

			// sign of adj positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj.content[j][i] = ((sign) * (float)(temp.GetDeterminant(3)));
		}
	}
	return adj;
}

Mat3D Mat3D::TransposeMatrix()
{
	float x[9] = { 0 };
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			x[i + j * 3] = content[i][j];
		}
	}

	return Mat3D{ x };
}

Mat3D Mat3D::Identity()
{
	return Mat3D(1);
}

void Mat3D::PrintMatrix()
{
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (content[j][i] >= 0.0f) printf(" ");
			std::cout << content[j][i];
		}
		printf("\n");
	}
	printf("\n");
}


// -----------------------   Mat4D    -----------------------

Mat4D::Mat4D(float diagonal)
{
	for (size_t i = 0; i < 4; i++) content[i][i] = diagonal;
}

Mat4D::Mat4D(const Mat4D& in)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			content[j][i] = in.content[j][i];
		}
	}
}

Mat4D::Mat4D(const float* data)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			content[j][i] = data[j * 4 + i];
		}
	}
}
Mat4D Mat4D::Multiply(Mat4D a) const
{
	Mat4D out;
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			float res = 0;
			for (size_t k = 0; k < 4; k++)
				res += content[j][k] * a.content[k][i];

			out.content[j][i] = res;
		}
	}
	return out;
}

Mat4D Mat4D::operator*(const Mat4D& in) const
{
	Mat4D out;
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			float res = 0;
			for (size_t k = 0; k < 4; k++)
				res += content[j][k] * in.content[k][i];

			out.content[j][i] = res;
		}
	}
	return out;
}

Vec4D Mat4D::operator*(const Vec4D& in) const
{
	Vec4D out;
	for (size_t i = 0; i < 4; i++)
	{
		float res = 0;
		for (size_t k = 0; k < 4; k++) res += content[i][k] * in[k];
		out[i] = res;
	}
	return out;
}

Mat4D Mat4D::operator+(const Mat4D& a) const
{
	Mat4D tmp;
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			tmp.content[j][i] = content[j][i] + a.content[j][i];
		}
	}
	return tmp;
}

Mat4D Mat4D::CreateXRotationMatrix(float angle)
{
	Mat4D out = Mat4D(1);
	float radA = math::ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(1, 1) = cosA;
	out.at(2, 1) = -sinA;
	out.at(1, 2) = sinA;
	out.at(2, 2) = cosA;
	return out;
}

Mat4D Mat4D::CreateYRotationMatrix(float angle)
{
	Mat4D out = Mat4D(1);
	float radA = math::ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(0, 0) = cosA;
	out.at(2, 0) = sinA;
	out.at(0, 2) = -sinA;
	out.at(2, 2) = cosA;
	return out;
}

Mat4D Mat4D::CreateZRotationMatrix(float angle)
{
	Mat4D out = Mat4D(1);
	float radA = math::ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(0, 0) = cosA;
	out.at(1, 0) = -sinA;
	out.at(0, 1) = sinA;
	out.at(1, 1) = cosA;
	return out;
}

Mat4D Mat4D::CreateScaleMatrix(const Vec3D& scale)
{
	Mat4D out;
	for (size_t i = 0; i < 3; i++) out.at((const unsigned char)i, (const unsigned char)i) = scale[i];
	out.content[3][3] = 1;
	return out;
}

Mat4D Mat4D::CreateTranslationMatrix(const Vec3D& translation)
{
	Mat4D out = Mat4D(1);
	for (size_t i = 0; i < 3; i++) out.at(3, (const unsigned char)i) = translation[i];
	return out;
}

Mat4D Mat4D::CreateRotationMatrix(const Vec3D& rotation)
{
	return CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z);
}

Mat4D Mat4D::CreateTransformMatrix(const Vec3D& position, const Vec3D& rotation, const Vec3D& scale)
{
	return CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
}

Mat4D Mat4D::CreatePerspectiveProjectionMatrix(float Near, float Far, float fov)
{
	float s = 1.0f / tanf(math::ToRadians(fov / 2.0f));
	float param1 = -Far / (Far - Near);
	float param2 = param1 * Near;
	Mat4D out;
	out.at(0, 0) = s;
	out.at(1, 1) = s;
	out.at(2, 2) = param1;
	out.at(3, 2) = -1;
	out.at(2, 3) = param2;
	return out;
}

Mat4D Mat4D::GetCofactor(int p, int q, int n)
{
	Mat4D mat;
	int i = 0, j = 0;
	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element
			//  which are not in given row and column
			if (row != p && col != q)
			{
				mat.content[i][j++] = content[row][col];

				// Row is filled, so increase row index and
				// reset col index
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
	return mat;
}

int Mat4D::GetDeterminant(int n)
{
	Mat4D a;
	int D = 0; // Initialize result

	//  Base case : if matrix contains single element
	if (n == 1)
		return (int)content[0][0];

	int sign = 1;  // To store sign multiplier

	 // Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of matrix[0][f]
		a = GetCofactor(0, f, n);
		D += sign * (int)content[0][f] * a.GetDeterminant(n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}

Vec3D Mat4D::GetPosition()
{
	return Vec3D(content[0][3], content[1][3], content[2][3]);
}

Vec3D Mat4D::GetRotation()
{
	Vec3D sca = GetScale();
	Mat4D rotMat;
	// Get Rotation Matrix.
	if (sca == Vec3D(0))
		sca = Vec3D(0.0001f);
	rotMat.at(0, 0) = at(0, 0) / sca.x;
	rotMat.at(0, 1) = at(0, 1) / sca.x;
	rotMat.at(0, 2) = at(0, 2) / sca.x;
	rotMat.at(1, 0) = at(1, 0) / sca.y;
	rotMat.at(1, 1) = at(1, 1) / sca.y;
	rotMat.at(1, 2) = at(1, 2) / sca.y;
	rotMat.at(2, 0) = at(2, 0) / sca.z;
	rotMat.at(2, 1) = at(2, 1) / sca.z;
	rotMat.at(2, 2) = at(2, 2) / sca.z;
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

Vec3D Mat4D::GetScale()
{
	// World Scale equal lenght of columns of the model matrix.
	float x = Vec3D(content[0][0], content[0][1], content[0][2]).GetLength();
	float y = Vec3D(content[1][0], content[1][1], content[1][2]).GetLength();
	float z = Vec3D(content[2][0], content[2][1], content[2][2]).GetLength();
	return Vec3D(x, y, z);
}

Mat4D Mat4D::CreateInverseMatrix()
{
	// Find determinant of matrix
	Mat4D inverse;
	int det = GetDeterminant(4);
	if (det == 0)
	{
		return 1;
	}

	// Find adjoint
	Mat4D adj = CreateAdjMatrix();

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			inverse.content[i][j] = adj.content[i][j] / float(det);

	return inverse;
}

Mat4D Mat4D::CreateAdjMatrix()
{
	// temp is used to store cofactors of matrix
	Mat4D temp;
	Mat4D adj;
	int sign = 1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// Get cofactor of matrix[i][j]
			temp = GetCofactor(i, j, 4);

			// sign of adj positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj.content[j][i] = (float)((sign) * (temp.GetDeterminant(3)));
		}
	}
	return adj;
}


Mat4D Mat4D::TransposeMatrix()
{
	float x[16] = { 0 };
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			x[i + j * 4] = content[i][j];
		}
	}

	return Mat4D{ x };
}

void Mat4D::PrintMatrix()
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (content[j][i] >= 0.0f) printf(" ");
			printf("%.2f", content[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

static void math::PrintMatrix(Mat4D a)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (a.content[j][i] >= 0.0f) printf(" ");
			printf("%.2f", a.content[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

Mat4D Mat4D::Identity()
{
	return Mat4D(1);
}


Vec3D Quaternion::ToEuler()
{
	Vec3D angles;
	// roll (x-axis rotation)
	float sinr_cosp = 2 * (float)(w * x + y * z);
	float cosr_cosp = 1 - 2 * (float)(x * x + y * y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2 * (float)(w * y - z * x);
	if (std::abs(sinp) >= 1)
		angles.y = (float)std::copysign(PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.y = std::asin(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = 2 * (float)(w * z + x * y);
	float cosy_cosp = 1 - 2 * (float)(y * y + z * z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}


void Quaternion::Print() const
{
	printf("Quaternion { %f, %f, %f, %f}\n", x, y, z, w);
}



namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(Math, m) {
	py::class_<Vec2D>(m, "Vec2D")
		.def(py::init<>(), "Vector2 Initialization")
		.def(py::init<float, float>(), "Vector2 Initialization")
		.def("__repr__", &Vec2D::ToString)
	
		.def(py::init<Vec2D>(), "Vector2 Initialization")
		.def(py::init<Int2D>(), "Vector2 Initialization")
		.def_readwrite("x", &Vec2D::x)
		.def_readwrite("y", &Vec2D::y)
		.def("__repr__", &Vec2D::ToString, "Vector2 Print")
		.def(py::self + py::self, "Vector2 Operator+")
		.def(py::self - py::self, "Vector2 Operator-")
		.def(py::self * py::self, "Vector2 Operator*")
		.def(py::self * float(), "Vector2 Operator* With float")
		.def(py::self / float(), "Vector2 Operator/ With Float")
		.def(py::self == py::self, "Vector2 Operator==")
		.def("LengthSquared", &Vec2D::LengthSquared)
		.def("GetDistanceFromPoint", &Vec2D::GetDistanceFromPoint)
		.def("GetLength", &Vec2D::GetLength)
		.def("IsCollinearWith", &Vec2D::IsCollinearWith)
		.def("DotProduct", &Vec2D::DotProduct)
		.def("CrossProduct", &Vec2D::CrossProduct)
		.def("UnitVector", &Vec2D::UnitVector, "Return Vec2 Unit Vector")
		.def("Negate", &Vec2D::Negate, "Return Vec2 Negate")
		.def("GetNormal", &Vec2D::GetNormal, "Return Vec2 Normal")
		.def("IsIntEquivalent", &Vec2D::IsIntEquivalent);

	py::class_<Vec3D>(m, "Vec3D")
		.def_readwrite("x", &Vec3D::x)
		.def_readwrite("y", &Vec3D::y)
		.def_readwrite("z", &Vec3D::z)

		.def(py::init<>())												//Vec3D() : x(0), y(0), z(0) {}
		.def(py::init<float, float, float>(), "Vector3 Initialization")	//Vec3D(float a, float b, float c) : x(a), y(b), z(c) {}
		.def(py::init<float>())											//Vec3D(float content) : x(content), y(content), z(content) {}
		.def(py::init<const Vec3D>())									//Vec3D(const Vec3D & in) : x(in.x), y(in.y), z(in.z) {}
		.def(py::init<Vec4D>())										//Vec3D(Vec4D a);

		//.def("assign", &Vec3D::operator=);

		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * py::self)
		.def(py::self * float())
		.def(py::self / float())

		.def(py::self == py::self)
		.def(py::self > py::self)
		.def(py::self < py::self)
		.def(py::self != py::self)

		.def("__getitem__", [](const Vec3D& a, size_t b) {
			return a[b];
		}, py::is_operator())
		.def("__setitem__", [](Vec3D& a, size_t b, float c) {
			a[b] = c;
		}, py::is_operator())
		

		.def(py::self += py::self)	//prob
		.def(py::self -= py::self)	//prob

		//.def("Print",&Vec3D::Print)
		.def("LengthSquared", &Vec3D::LengthSquared)
		.def("GetLength", &Vec3D::GetLength)
		.def("IsCollinearWith", &Vec3D::IsCollinearWith)
		.def("GetDistanceBetween", &Vec3D::GetDistanceBetween)
		.def("DotProduct", &Vec3D::DotProduct)
		.def("CrossProduct", &Vec3D::CrossProduct)
		.def("UnitVector", &Vec3D::UnitVector)
		.def("Negate", &Vec3D::Negate)
		.def("IsIntEquivalent", &Vec3D::IsIntEquivalent)
		.def("ToRad", &Vec3D::ToRad)
		.def("ToDeegree", &Vec3D::ToDeegree)
		.def("Clamp", &Vec3D::Clamp)

		.def_static("ClampMagnitude", &Vec3D::ClampMagnitude)
		.def("Normalize", &Vec3D::Normalize)
		.def("ToQuaternion", &Vec3D::ToQuaternion)

		.def("__repr__", &Vec3D::ToString);

	py::class_<Mat4D>(m, "Mat4D")
		.def(py::init<>(), "Mat4D Initialization")
		.def(py::init<>([](float a) { return Mat4D(a); }))
		.def(py::init<Mat4D>())
		.def("__repr__", &Mat4D::ToString, "Mat4D Print")
		.def("at", &Mat4D::at)
		.def_static("Identity", &Mat4D::Identity)
		.def_static("CreateTransformMatrix", &Mat4D::CreateTransformMatrix)
		.def_static("CreateTranslationMatrix", &Mat4D::CreateTranslationMatrix)
		.def_static("CreateScaleMatrix", &Mat4D::CreateScaleMatrix)
		.def_static("CreateXRotationMatrix", &Mat4D::CreateXRotationMatrix)
		.def_static("CreateYRotationMatrix", &Mat4D::CreateYRotationMatrix)
		.def_static("CreateZRotationMatrix", &Mat4D::CreateZRotationMatrix)
		.def_static("CreateRotationMatrix", &Mat4D::CreateRotationMatrix)
		.def_static("CreatePerspectiveProjectionMatrix", &Mat4D::CreatePerspectiveProjectionMatrix)
		.def("GetPosition", &Mat4D::GetPosition)
		.def("GetRotation", &Mat4D::GetRotation)
		.def("GetScale", &Mat4D::GetScale)
		.def("CreateInverseMatrix", &Mat4D::CreateInverseMatrix)
		.def("CreateAdjMatrix", &Mat4D::CreateAdjMatrix)
		.def("GetCofactor", &Mat4D::GetCofactor)
		.def("GetDeterminant", &Mat4D::GetDeterminant)
		.def("TransposeMatrix", &Mat4D::TransposeMatrix)
		.def(py::self * py::self)
		.def(py::self * Vec4D())
		.def(py::self + py::self);

	py::class_<Vec4D>(m, "Vec4D")
		.def(py::init<>(), "Vector4 Initialization")
		.def(py::init<float>(), "Vector4 Initialization")
		.def(py::init<float, float, float, float>(), "Vector4 Initialization")
		.def(py::init<Vec3D, float>(), "Vector4 Initialization")
		.def(py::init<Vec4D>(), "Vector4 Initialization")
		.def_readwrite("x", &Vec4D::x)
		.def_readwrite("y", &Vec4D::y)
		.def_readwrite("z", &Vec4D::z)
		.def_readwrite("w", &Vec4D::w)
		.def("__repr__", &Vec4D::ToString, "Vector4 Print")
		.def(py::self + py::self, "Vector4 Operator+")
		.def(py::self - py::self, "Vector4 Operator-")
		.def(py::self * py::self, "Vector4 Operator*")
		.def(py::self * float(), "Vector4 Operator* With float")
		.def(py::self / float(), "Vector4 Operator/ With Float")
		//.def(py::self == py::self, "Vector4 Operator==")
		.def("LengthSquared", &Vec4D::LengthSquared)
		.def("GetLength", &Vec4D::GetLength)
		.def("IsCollinearWith", &Vec4D::IsCollinearWith)
		.def("DotProduct", &Vec4D::DotProduct)
		.def("CrossProduct", &Vec4D::CrossProduct)
		.def("UnitVector", &Vec4D::UnitVector, "Return Vec4 Unit Vector")
		.def("Negate", &Vec4D::Negate, "Return Vec4 Negate")
		.def("Homogenize", &Vec4D::Homogenize, "Return Vec4 Homogenized")
		.def("IsIntEquivalent", &Vec4D::IsIntEquivalent);
	m.def("Cut", &math::Cut);
	m.def("Mod", &math::Mod);
	m.def("ToRadians", &math::ToRadians);
	m.def("ToDegrees", &math::ToDegrees);

	py::class_<Int3D>(m, "Int3D")
		.def(py::init<>())
		.def_readwrite("x", &Int3D::x)
		.def_readwrite("y", &Int3D::y)
		.def_readwrite("z", &Int3D::z);

}