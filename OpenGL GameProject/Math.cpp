#include "Math.h"

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);
const Vector2 Vector2::NegaUnitX(-1.0f, 0.0f);
const Vector2 Vector2::NegaUnitY(0.0f, -1.0f);

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.f);
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NegaUnitX(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NegaUnitY(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NegaUnitZ(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Infinity(Math::Infinity, Math::Infinity, Math::Infinity);
const Vector3 Vector3::NegaInfinity(Math::NegaInfinity, Math::NegaInfinity, Math::NegaInfinity);

static float m3Ident[3][3] =
{
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};
const Matrix3 Matrix3::Identity(m3Ident);

static float m4Ident[4][4] =
{
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Matrix4 Matrix4::Identity(m4Ident);

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

Vector2 Vector2::Transform(const Vector2& vec, const Matrix3& mat, float w /*= 1.0f*/)
{
	Vector2 returnValue;
	returnValue.x = vec.x * mat.matrix[0][0] + vec.y * mat.matrix[1][0] + w * mat.matrix[2][0];
	returnValue.y = vec.x * mat.matrix[0][1] + vec.y * mat.matrix[1][1] + w * mat.matrix[2][1];
	//ignore w since we aren't returning a new value for it...
	return returnValue;
}

Vector3 Vector3::Transform(const Vector3& vec, const Matrix4& mat, float w /*= 1.0f*/)
{
	Vector3 returnValue;
	returnValue.x = vec.x * mat.matrix[0][0] + vec.y * mat.matrix[1][0] +
		vec.z * mat.matrix[2][0] + w * mat.matrix[3][0];
	returnValue.y = vec.x * mat.matrix[0][1] + vec.y * mat.matrix[1][1] +
		vec.z * mat.matrix[2][1] + w * mat.matrix[3][1];
	returnValue.z = vec.x * mat.matrix[0][2] + vec.y * mat.matrix[1][2] +
		vec.z * mat.matrix[2][2] + w * mat.matrix[3][2];
	//ignore w since we aren't returning a new value for it...
	return returnValue;
}

// This will transform the vector and renormalize the w component
Vector3 Vector3::TransformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w /*= 1.0f*/)
{
	Vector3 returnValue;
	returnValue.x = vec.x * mat.matrix[0][0] + vec.y * mat.matrix[1][0] +
		vec.z * mat.matrix[2][0] + w * mat.matrix[3][0];
	returnValue.y = vec.x * mat.matrix[0][1] + vec.y * mat.matrix[1][1] +
		vec.z * mat.matrix[2][1] + w * mat.matrix[3][1];
	returnValue.z = vec.x * mat.matrix[0][2] + vec.y * mat.matrix[1][2] +
		vec.z * mat.matrix[2][2] + w * mat.matrix[3][2];
	float transformedW = vec.x * mat.matrix[0][3] + vec.y * mat.matrix[1][3] +
		vec.z * mat.matrix[2][3] + w * mat.matrix[3][3];
	if (!Math::NearZero(Math::Abs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		returnValue *= transformedW;
	}
	return returnValue;
}

// Transform a Vector3 by a quaternion
Vector3 Vector3::Transform(const Vector3& v, const Quaternion& q)
{
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3 qv(q.x, q.y, q.z);
	Vector3 returnValue = v;
	returnValue += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w * v);
	return returnValue;
}

void Matrix4::Invert()
{

	float tmp[12];
	float src[16];
	float dst[16];
	float det;

	// Transpose matrix
	src[0] = matrix[0][0];
	src[4] = matrix[0][1];
	src[8] = matrix[0][2];
	src[12] = matrix[0][3];

	// row 2 to col 2
	src[1] = matrix[1][0];
	src[5] = matrix[1][1];
	src[9] = matrix[1][2];
	src[13] = matrix[1][3];

	// row 3 to col 3
	src[2] = matrix[2][0];
	src[6] = matrix[2][1];
	src[10] = matrix[2][2];
	src[14] = matrix[2][3];

	// row 4 to col 4
	src[3] = matrix[3][0];
	src[7] = matrix[3][1];
	src[11] = matrix[3][2];
	src[15] = matrix[3][3];

	// Calculate cofactors
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	// Calculate determinant
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	// Inverse of matrix is divided by determinant
	det = 1 / det;
	for (int j = 0; j < 16; j++)
	{
		dst[j] *= det;
	}

	// Set it back
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i][j] = dst[i * 4 + j];
		}
	}
}

Matrix4 Matrix4::CreateFromQuaternion(const class Quaternion& q)
{
	float matrix[4][4];

	matrix[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	matrix[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	matrix[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	matrix[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	matrix[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
	matrix[1][3] = 0.0f;

	matrix[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	matrix[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	matrix[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	return Matrix4(matrix);
}
