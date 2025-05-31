#pragma once

class Vec3
{
private:
	float e[3]{};

public:
	Vec3()
		: e{0, 0, 0}
	{
	}

	Vec3(float e0, float e1, float e2)
		: e{e0, e1, e2}
	{
	}

	// getters
	float getX() const { return e[0]; }
	float getY() const { return e[1]; }
	float getZ() const { return e[2]; }

	// setters
	void setX(float x) { e[0] = x; }
	void setY(float y) { e[1] = y; }
	void setZ(float z) { e[2] = z; }

	// multiplying vec by a negative
	Vec3 operator- () const
	{
		return Vec3(-e[0], -e[1], -e[2]);
	}

	// non-const subcript
	float& operator[](int index)
	{
		return e[index];
	}

	// const subcript
	const float& operator[](int index) const
	{
		return e[index];
	}

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];

		return *this;
	}

	// multiplying vec by scalar
	Vec3& operator*=(float d)
	{
		e[0] *= d;
		e[1] *= d;
		e[2] *= d;

		return *this;
	}

	// dividing vec by scalar (same as mult my reciprocal)
	Vec3& operator/=(float d)
	{
		return *this *= (1 / d);
	}

	// used for calculating the length of vec
	// was using std::pow, but not as efficient
	float lengthSquared() const
	{
		return ((e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]));
	}

	float length() const
	{
		return std::sqrt(lengthSquared());
	}
};

// better clarity when dealing with colors vs points
using point3 = Vec3;

// print the vec
inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.getX() << " " << v.getY() << " " << v.getZ();
}

// add two vec
inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.getX() + v2.getX(), v1.getY() + v2.getY(), v1.getZ() + v2.getZ());
}

// subtract two vec
inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());
}

// Hadamard product (never learned this in lin alg before :o)
inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.getX() * v2.getX(), v1.getY() * v2.getY(), v1.getZ() * v2.getZ());
}

// multiplying scalar by vec
inline Vec3 operator*(const float d, const Vec3& v)
{
	return Vec3(d * v.getX(), d * v.getY(), d * v.getZ());
}

// allows us to write the scalar on the right
inline Vec3 operator*(const Vec3& v, const float d)
{
	return d * v;
}

// dividing is the same as mult by reciprocal
inline Vec3 operator/(const Vec3& v, const float d)
{
	return ((1 / d) * v);
}

// dot product
inline float dot(const Vec3& v1, const Vec3 v2)
{
	return ((v1.getX() * v2.getX()) + (v1.getY() * v2.getY()) + (v1.getZ() * v2.getZ()));
}

// cross product
inline Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3((v1.getY() * v2.getZ()) - (v1.getZ() * v2.getY()),
			   (v1.getZ() * v2.getX()) - (v1.getX() * v2.getZ()),
			   (v1.getX() * v2.getY()) - (v1.getY() * v2.getX()));
}

// unit vector
inline Vec3 unit(const Vec3& v)
{
	return Vec3(v / v.length());
}