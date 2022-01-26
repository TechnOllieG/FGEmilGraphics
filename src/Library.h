#pragma once

#define EQUALITY_MARGIN 1.e-4f
#define TAU 6.28318530717959f
constexpr float deg2rad = TAU / 360.f;

struct Vector2
{
	float x = 0.f;
	float y = 0.f;

	Vector2()
	{
		x = 0.f;
		y = 0.f;
	}

	Vector2(float inBoth)
	{
		x = inBoth;
		y = inBoth;
	}

	Vector2(float inX, float inY)
	{
		x = inX;
		y = inY;
	}

	Vector2 operator*(float a)
	{
		return Vector2(x * a, y * a);
	}

	Vector2 operator/(float a)
	{
		return Vector2(x / a, y / a);
	}

	Vector2 operator+(Vector2 rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	void operator+=(Vector2 rhs)
	{
		*this = Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(Vector2 rhs)
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	void operator-=(Vector2 rhs)
	{
		*this = Vector2(x - rhs.x, y - rhs.y);
	}

	bool operator==(Vector2 rhs)
	{
		return abs(x - rhs.x) < EQUALITY_MARGIN && abs(y - rhs.y) < EQUALITY_MARGIN;
	}

	bool operator!=(Vector2 rhs)
	{
		return !(*this == rhs);
	}

	float sqrMagnitude()
	{
		return x * x + y * y;
	}

	float magnitude()
	{
		return sqrt(sqrMagnitude());
	}

	Vector2 normalized()
	{
		float mag = magnitude();
		return *this / mag;
	}

	void Normalize()
	{
		*this = normalized();
	}

	float static Distance(Vector2 a, Vector2 b)
	{
		return (a - b).magnitude();
	}

	float static SqrDistance(Vector2 a, Vector2 b)
	{
		return (a - b).sqrMagnitude();
	}
};

struct Vector3
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	Vector3()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	Vector3(float inBoth)
	{
		x = inBoth;
		y = inBoth;
		z = inBoth;
	}

	Vector3(float inX, float inY, float inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}

	Vector3 operator*(float a)
	{
		return Vector3(x * a, y * a, z * a);
	}

	Vector3 operator/(float a)
	{
		return Vector3(x / a, y / a, z / a);
	}

	Vector3 operator+(Vector3 rhs)
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	void operator+=(Vector3 rhs)
	{
		*this = Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3 operator-(Vector3 rhs)
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	void operator-=(Vector3 rhs)
	{
		*this = Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	bool operator==(Vector3 rhs)
	{
		return abs(x - rhs.x) < EQUALITY_MARGIN && abs(y - rhs.y) < EQUALITY_MARGIN && abs(z - rhs.z) < EQUALITY_MARGIN;
	}

	bool operator!=(Vector3 rhs)
	{
		return !(*this == rhs);
	}

	float sqrMagnitude()
	{
		return x * x + y * y + z * z;
	}

	float magnitude()
	{
		return sqrt(sqrMagnitude());
	}

	Vector3 normalized()
	{
		float mag = magnitude();
		return *this / mag;
	}

	void Normalize()
	{
		*this = normalized();
	}

	float static Distance(Vector3 a, Vector3 b)
	{
		return (a - b).magnitude();
	}

	float static SqrDistance(Vector3 a, Vector3 b)
	{
		return (a - b).sqrMagnitude();
	}
};