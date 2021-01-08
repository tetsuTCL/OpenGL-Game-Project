#pragma once
#include <cmath>
#include <memory.h>
#include <limits>

namespace Math
{
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegaInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	inline bool NearZero(float val, float epsilon = 0.001f)
	{
		if (fabs(val) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	T Max(const T& a, const T& b)
	{
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b)
	{
		return (a < b ? a : b);
	}

	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Min(upper, Max(lower, value));
	}

	inline float Abs(float value)
	{
		return fabs(value);
	}

	inline float Cos(float angle)
	{
		return cosf(angle);
	}

	inline float Sin(float angle)
	{
		return sinf(angle);
	}

	inline float Tan(float angle)
	{
		return tanf(angle);
	}

	inline float Acos(float value)
	{
		return acosf(value);
	}

	inline float Atan2(float y, float x)
	{
		return atan2f(y, x);
	}

	inline float Cot(float angle)
	{
		return 1.0f / Tan(angle);
	}

	inline float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	inline float Fmod(float numer, float denom)
	{
		return fmod(numer, denom);
	}
}

//2D Vector
class Vector2
{
public:
	float X;
	float Y;

	Vector2()
		:X(0.0f)
		, Y(0.0f)
	{
	
	}

	explicit Vector2(float inX, float inY)
		:X(inX)
		, Y(inY)
	{
	
	}

	void Set(float inX, float inY)
	{
		X = inX;
		Y = inY;
	}

	friend Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X + b.X, a.Y + b.Y);
	}

	friend Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X - b.X, a.Y - b.Y);
	}

	//friend Vector2 operator-(const Vector2& a)
	//{
	//	return Vector2(-a.X, -a.Y);
	//}

	friend Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X * b.X, a.Y * b.Y);
	}

	// Scalar multiplication
	friend Vector2 operator*(const Vector2& vec, float scalar)
	{
		return Vector2(vec.X * scalar, vec.Y * scalar);
	}

	// Scalar multiplication
	friend Vector2 operator*(float scalar, const Vector2& vec)
	{
		return Vector2(vec.X * scalar, vec.Y * scalar);
	}

	// Scalar *=
	Vector2& operator*=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}

	// Vector +=
	Vector2& operator+=(const Vector2& right)
	{
		X += right.X;
		Y += right.Y;
		return *this;
	}

	// Vector -=
	Vector2& operator-=(const Vector2& right)
	{
		X -= right.X;
		Y -= right.Y;
		return *this;
	}

	// Length squared of vector
	float LengthSq() const
	{
		return (X * X + Y * Y);
	}

	// Length of vector
	float Length() const
	{
		return (Math::Sqrt(LengthSq()));
	}

	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		if (length != 0)
		{
			X /= length;
			Y /= length;
		}
	}

	//Normalize the provided vector
	static Vector2 Normalize(const Vector2& vec)
	{
		Vector2 temp = vec;
		temp.Normalize();
		return temp;
	}


	static float Dot(const Vector2& a, const Vector2& b)
	{
		return (a.X * b.X + a.Y * b.Y);
	}

	//Lerp from A to B by f
	static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
	{
		return Vector2(a + f * (b - a));
	}

	static Vector2 Reflect(const Vector2& v, const Vector2& n)
	{
		return v - 2.0f * Vector2::Dot(v, n) * n;
	}

	//Transform vector by matrix
	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 NegaUnitX;
	static const Vector2 NegaUnitY;
};

//3D Vector
class Vector3
{
public:
	float X;
	float Y;
	float Z;

	Vector3()
		:X(0.0f)
		, Y(0.0f)
		, Z(0.0f)
	{
	
	}

	explicit Vector3(float inX, float inY, float inZ)
		:X(inX)
		, Y(inY)
		, Z(inZ)
	{
	
	}

	//Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&X);
	}

	//Set all three components in one line
	void Set(float inX, float inY, float inZ)
	{
		X = inX;
		Y = inY;
		Z = inZ;
	}

	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
	}

	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
	}

	friend Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
	}

	friend Vector3 operator*(const Vector3& vec, float scalar)
	{
		return Vector3(vec.X * scalar, vec.Y * scalar, vec.Z * scalar);
	}


	friend Vector3 operator*(float scalar, const Vector3& vec)
	{
		return Vector3(vec.X * scalar, vec.Y * scalar, vec.Z * scalar);
	}


	Vector3& operator*=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	//Vector +=
	Vector3& operator+=(const Vector3& right)
	{
		X += right.X;
		Y += right.Y;
		Z += right.Z;
		return *this;
	}

	//Vector -=
	Vector3& operator-=(const Vector3& right)
	{
		X -= right.X;
		Y -= right.Y;
		Z -= right.Z;
		return *this;
	}


	float LengthSq() const
	{
		return (X * X + Y * Y + Z * Z);
	}

	float Length() const
	{
		return (Math::Sqrt(LengthSq()));
	}

	//Normalize this vector
	void Normalize()
	{
		float length = Length();
		if (length != 0)
		{
			X /= length;
			Y /= length;
			Z /= length;
		}
	}

	//Normalize the provided vector
	static Vector3 Normalize(const Vector3& vec)
	{
		Vector3 temp = vec;
		temp.Normalize();
		return temp;
	}

	//Dot product between two vectors (a dot b)
	static float Dot(const Vector3& a, const Vector3& b)
	{
		return (a.X * b.X + a.Y * b.Y + a.Z * b.Z);
	}

	//Cross product between two vectors (a cross b)
	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		Vector3 temp;
		temp.X = a.Y * b.Z - a.Z * b.Y;
		temp.Y = a.Z * b.X - a.X * b.Z;
		temp.Z = a.X * b.Y - a.Y * b.X;
		return temp;
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
	{
		return Vector3(a + f * (b - a));
	}


	static Vector3 Reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2.0f * Vector3::Dot(v, n) * n;
	}

	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
	
	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);


	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegaUnitX;
	static const Vector3 NegaUnitY;
	static const Vector3 NegaUnitZ;
	static const Vector3 Infinity;
	static const Vector3 NegaInfinity;
};


class Matrix3
{
public:
	float matrix[3][3];

	Matrix3()
	{
		*this = Matrix3::Identity;
	}

	explicit Matrix3(float inMat[3][3])
	{
		memcpy(matrix, inMat, 9 * sizeof(float));
	}

	//Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}

	//Matrix multiplication
	friend Matrix3 operator*(const Matrix3& left, const Matrix3& right)
	{
		Matrix3 returnValue;
		//row 0
		returnValue.matrix[0][0] =
			left.matrix[0][0] * right.matrix[0][0] +
			left.matrix[0][1] * right.matrix[1][0] +
			left.matrix[0][2] * right.matrix[2][0];

		returnValue.matrix[0][1] =
			left.matrix[0][0] * right.matrix[0][1] +
			left.matrix[0][1] * right.matrix[1][1] +
			left.matrix[0][2] * right.matrix[2][1];

		returnValue.matrix[0][2] =
			left.matrix[0][0] * right.matrix[0][2] +
			left.matrix[0][1] * right.matrix[1][2] +
			left.matrix[0][2] * right.matrix[2][2];

		//row 1
		returnValue.matrix[1][0] =
			left.matrix[1][0] * right.matrix[0][0] +
			left.matrix[1][1] * right.matrix[1][0] +
			left.matrix[1][2] * right.matrix[2][0];

		returnValue.matrix[1][1] =
			left.matrix[1][0] * right.matrix[0][1] +
			left.matrix[1][1] * right.matrix[1][1] +
			left.matrix[1][2] * right.matrix[2][1];

		returnValue.matrix[1][2] =
			left.matrix[1][0] * right.matrix[0][2] +
			left.matrix[1][1] * right.matrix[1][2] +
			left.matrix[1][2] * right.matrix[2][2];
		//row2
		returnValue.matrix[2][0] =
			left.matrix[2][0] * right.matrix[0][0] +
			left.matrix[2][1] * right.matrix[1][0] +
			left.matrix[2][2] * right.matrix[2][0];

		returnValue.matrix[2][1] =
			left.matrix[2][0] * right.matrix[0][1] +
			left.matrix[2][1] * right.matrix[1][1] +
			left.matrix[2][2] * right.matrix[2][1];

		returnValue.matrix[2][2] =
			left.matrix[2][0] * right.matrix[0][2] +
			left.matrix[2][1] * right.matrix[1][2] +
			left.matrix[2][2] * right.matrix[2][2];

		return returnValue;
	}

	Matrix3& operator*=(const Matrix3& right)
	{
		*this = *this * right;
		return *this;
	}

	static Matrix3 CreateScale(float xScale, float yScale)
	{
		float temp[3][3] =
		{
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}

	static Matrix3 CreateScale(const Vector2& scaleVector)
	{
		return CreateScale(scaleVector.X, scaleVector.Y);
	}

	static Matrix3 CreateScale(float scale)
	{
		return CreateScale(scale, scale);
	}

	static Matrix3 CreateRotation(float theta)
	{

		float cosine = Math::Cos(theta);
		float temp[3][3] =
		{
			{ cosine, Math::Sin(theta), 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}

	static Matrix3 CreateTranslation(const Vector2& trans)
	{
		float temp[3][3] =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.X, trans.Y, 1.0f },
		};
		return Matrix3(temp);
	}

	static const Matrix3 Identity;
};

class Matrix4
{
public:
	float matrix[4][4];

	Matrix4()
	{
		*this = Matrix4::Identity;
	}

	explicit Matrix4(float inMat[4][4])
	{
		memcpy(matrix, inMat, 16 * sizeof(float));
	}

	//Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}

	//Matrix multiplication
	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 returnValue;
		//row0
		returnValue.matrix[0][0] =
			a.matrix[0][0] * b.matrix[0][0] +
			a.matrix[0][1] * b.matrix[1][0] +
			a.matrix[0][2] * b.matrix[2][0] +
			a.matrix[0][3] * b.matrix[3][0];

		returnValue.matrix[0][1] =
			a.matrix[0][0] * b.matrix[0][1] +
			a.matrix[0][1] * b.matrix[1][1] +
			a.matrix[0][2] * b.matrix[2][1] +
			a.matrix[0][3] * b.matrix[3][1];

		returnValue.matrix[0][2] =
			a.matrix[0][0] * b.matrix[0][2] +
			a.matrix[0][1] * b.matrix[1][2] +
			a.matrix[0][2] * b.matrix[2][2] +
			a.matrix[0][3] * b.matrix[3][2];

		returnValue.matrix[0][3] =
			a.matrix[0][0] * b.matrix[0][3] +
			a.matrix[0][1] * b.matrix[1][3] +
			a.matrix[0][2] * b.matrix[2][3] +
			a.matrix[0][3] * b.matrix[3][3];

		//row 1
		returnValue.matrix[1][0] =
			a.matrix[1][0] * b.matrix[0][0] +
			a.matrix[1][1] * b.matrix[1][0] +
			a.matrix[1][2] * b.matrix[2][0] +
			a.matrix[1][3] * b.matrix[3][0];

		returnValue.matrix[1][1] =
			a.matrix[1][0] * b.matrix[0][1] +
			a.matrix[1][1] * b.matrix[1][1] +
			a.matrix[1][2] * b.matrix[2][1] +
			a.matrix[1][3] * b.matrix[3][1];

		returnValue.matrix[1][2] =
			a.matrix[1][0] * b.matrix[0][2] +
			a.matrix[1][1] * b.matrix[1][2] +
			a.matrix[1][2] * b.matrix[2][2] +
			a.matrix[1][3] * b.matrix[3][2];

		returnValue.matrix[1][3] =
			a.matrix[1][0] * b.matrix[0][3] +
			a.matrix[1][1] * b.matrix[1][3] +
			a.matrix[1][2] * b.matrix[2][3] +
			a.matrix[1][3] * b.matrix[3][3];

		//row2
		returnValue.matrix[2][0] =
			a.matrix[2][0] * b.matrix[0][0] +
			a.matrix[2][1] * b.matrix[1][0] +
			a.matrix[2][2] * b.matrix[2][0] +
			a.matrix[2][3] * b.matrix[3][0];

		returnValue.matrix[2][1] =
			a.matrix[2][0] * b.matrix[0][1] +
			a.matrix[2][1] * b.matrix[1][1] +
			a.matrix[2][2] * b.matrix[2][1] +
			a.matrix[2][3] * b.matrix[3][1];

		returnValue.matrix[2][2] =
			a.matrix[2][0] * b.matrix[0][2] +
			a.matrix[2][1] * b.matrix[1][2] +
			a.matrix[2][2] * b.matrix[2][2] +
			a.matrix[2][3] * b.matrix[3][2];

		returnValue.matrix[2][3] =
			a.matrix[2][0] * b.matrix[0][3] +
			a.matrix[2][1] * b.matrix[1][3] +
			a.matrix[2][2] * b.matrix[2][3] +
			a.matrix[2][3] * b.matrix[3][3];

		//row 3
		returnValue.matrix[3][0] =
			a.matrix[3][0] * b.matrix[0][0] +
			a.matrix[3][1] * b.matrix[1][0] +
			a.matrix[3][2] * b.matrix[2][0] +
			a.matrix[3][3] * b.matrix[3][0];

		returnValue.matrix[3][1] =
			a.matrix[3][0] * b.matrix[0][1] +
			a.matrix[3][1] * b.matrix[1][1] +
			a.matrix[3][2] * b.matrix[2][1] +
			a.matrix[3][3] * b.matrix[3][1];

		returnValue.matrix[3][2] =
			a.matrix[3][0] * b.matrix[0][2] +
			a.matrix[3][1] * b.matrix[1][2] +
			a.matrix[3][2] * b.matrix[2][2] +
			a.matrix[3][3] * b.matrix[3][2];

		returnValue.matrix[3][3] =
			a.matrix[3][0] * b.matrix[0][3] +
			a.matrix[3][1] * b.matrix[1][3] +
			a.matrix[3][2] * b.matrix[2][3] +
			a.matrix[3][3] * b.matrix[3][3];

		return returnValue;
	}

	Matrix4& operator*=(const Matrix4& right)
	{
		*this = *this * right;
		return *this;
	}

	void Invert();

	Vector3 GetTranslation() const
	{
		return Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}

	Vector3 GetXAxis() const
	{
		return Vector3::Normalize(Vector3(matrix[0][0], matrix[0][1], matrix[0][2]));
	}

	Vector3 GetYAxis() const
	{
		return Vector3::Normalize(Vector3(matrix[1][0], matrix[1][1], matrix[1][2]));
	}

	Vector3 GetZAxis() const
	{
		return Vector3::Normalize(Vector3(matrix[2][0], matrix[2][1], matrix[2][2]));
	}

	Vector3 GetScale() const
	{
		Vector3 returnValue;
		returnValue.X = Vector3(matrix[0][0], matrix[0][1], matrix[0][2]).Length();
		returnValue.Y = Vector3(matrix[1][0], matrix[1][1], matrix[1][2]).Length();
		returnValue.Z = Vector3(matrix[2][0], matrix[2][1], matrix[2][2]).Length();
		return returnValue;
	}

	static Matrix4 CreateScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateScale(const Vector3& scaleVector)
	{
		return CreateScale(scaleVector.X, scaleVector.Y, scaleVector.Z);
	}

	//Create a scale matrix with a uniform factor
	static Matrix4 CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	//Rotation about x-axis
	static Matrix4 CreateRotationX(float theta)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ 0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	//Rotation about y-axis
	static Matrix4 CreateRotationY(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	//Rotation about z-axis
	static Matrix4 CreateRotationZ(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	//Create a rotation matrix from a quaternion
	static Matrix4 CreateFromQuaternion(const class Quaternion& q);

	static Matrix4 CreateTranslation(const Vector3& trans)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.X, trans.Y, trans.Z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		Vector3 zAxis = Vector3::Normalize(target - eye);
		Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
		Vector3 yAxis = Vector3::Normalize(Vector3::Cross(zAxis, xAxis));
		Vector3 trans;
		trans.X = -Vector3::Dot(xAxis, eye);
		trans.Y = -Vector3::Dot(yAxis, eye);
		trans.Z = -Vector3::Dot(zAxis, eye);

		float temp[4][4] =
		{
			{ xAxis.X, yAxis.X, zAxis.X, 0.0f },
			{ xAxis.Y, yAxis.Y, zAxis.Y, 0.0f },
			{ xAxis.Z, yAxis.Z, zAxis.Z, 0.0f },
			{ trans.X, trans.Y, trans.Z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return Matrix4(temp);
	}

	static const Matrix4 Identity;
};


class Quaternion
{
public:
	float X;
	float Y;
	float Z;
	float W;

	Quaternion()
	{
		*this = Quaternion::Identity;
	}


	explicit Quaternion(float inX, float inY, float inZ, float inW)
	{
		Set(inX, inY, inZ, inW);
	}

	explicit Quaternion(const Vector3& axis, float angle)
	{
		float scalar = Math::Sin(angle / 2.0f);
		X = axis.X * scalar;
		Y = axis.Y * scalar;
		Z = axis.Z * scalar;
		W = Math::Cos(angle / 2.0f);
	}

	void Set(float inX, float inY, float inZ, float inW)
	{
		X = inX;
		Y = inY;
		Z = inZ;
		W = inW;
	}

	void Conjugate()
	{
		X *= -1.0f;
		Y *= -1.0f;
		Z *= -1.0f;
	}

	float LengthSq() const
	{
		return (X * X + Y * Y + Z * Z + W * W);
	}

	float Length() const
	{
		return Math::Sqrt(LengthSq());
	}

	void Normalize()
	{
		float length = Length();
		if (length != 0)
		{
			X /= length;
			Y /= length;
			Z /= length;
			W /= length;
		}
	}

	static Quaternion Normalize(const Quaternion& q)
	{
		Quaternion returnValue = q;
		returnValue.Normalize();
		return returnValue;
	}

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f)
	{
		Quaternion returnValue;
		returnValue.X = Math::Lerp(a.X, b.X, f);
		returnValue.Y = Math::Lerp(a.Y, b.Y, f);
		returnValue.Z = Math::Lerp(a.Z, b.Z, f);
		returnValue.W = Math::Lerp(a.W, b.W, f);
		returnValue.Normalize();
		return returnValue;
	}

	static float Dot(const Quaternion& a, const Quaternion& b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
	}

	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f)
	{
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else
		{
			//Use linear interpolation if the quaternions
			//are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion returnValue;
		returnValue.X = scale0 * a.X + scale1 * b.X;
		returnValue.Y = scale0 * a.Y + scale1 * b.Y;
		returnValue.Z = scale0 * a.Z + scale1 * b.Z;
		returnValue.W = scale0 * a.W + scale1 * b.W;
		returnValue.Normalize();
		return returnValue;
	}


	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion returnValue;

		//Vector component is:
		//ps * qv + qs * pv + pv x qv
		Vector3 qv(q.X, q.Y, q.Z);
		Vector3 pv(p.X, p.Y, p.Z);
		Vector3 newVec = p.W * qv + q.W * pv + Vector3::Cross(pv, qv);
		returnValue.X = newVec.X;
		returnValue.Y = newVec.Y;
		returnValue.Z = newVec.Z;

		//Scalar component is:
		//ps * qs - pv . qv
		returnValue.W = p.W * q.W - Vector3::Dot(pv, qv);

		return returnValue;
	}

	static const Quaternion Identity;
};

namespace Color
{
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(1.0f, 1.0f, 1.0f);
	static const Vector3 Red(1.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 1.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 1.0f);
	static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
	static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
	static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
	static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
	static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}
