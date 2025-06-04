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

	// return a vec where components are randomized between 0-1
	static Vec3 randomVec()
	{
		return Vec3(generateRandom(), generateRandom(), generateRandom());
	}

	// return a vec where components are random #s in designated boundary
	static Vec3 randomVec(float min, float max)
	{
		return Vec3(generateRandom(min, max), generateRandom(min, max), generateRandom(min, max));
	}

	// check to see if every component in a vec is near zero
	bool nearZero() const
	{
		return ((std::fabs(e[0]) < 1e-8) && (std::fabs(e[1]) < 1e-8) && (std::fabs(e[2]) < 1e-8));
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

// generate a random unit vector in sphere
inline Vec3 randomUnit()
{
	while (true)
	{
		auto randomPoint{ Vec3::randomVec(-1.0f, 1.0f) };
		float squaredRandomPoint{ randomPoint.lengthSquared() };

		// protect against floating point precision issues and make sure point is in sphere
		if (squaredRandomPoint > 1e-160 && squaredRandomPoint <= 1.0f)
		{
			return unit(randomPoint);
		}
	}	
}

// invert vec if it not on correct hemisphere
inline Vec3 randomOnHemisphere(const Vec3& normal)
{
	Vec3 randomVec{ randomUnit() };
	
	// make sure it is on correct hemisphere
	return (dot(randomVec, normal) > 0.0) ? randomVec : -randomVec;
}

// find reflected ray 
inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return (v - (2 * dot(v, n)) * n);
}

// find refracted ray for dielectric materials
inline Vec3 refract(float refractiveIndexRatio, const Vec3& r, const Vec3& n)
{
	auto rayPerp{ (refractiveIndexRatio * (r + (dot(-r, n)) * n)) };
	auto rayParallel{ -std::sqrt(1 - std::fabs(rayPerp.lengthSquared())) * n };

	return rayPerp + rayParallel;
}