#pragma once

#include "vec3.h"

class Ray
{
private:
	point3 m_origin{};
	Vec3 m_direction{};

public:
	Ray()
	{
	}

	Ray(const point3& origin, const Vec3& direction)
		: m_origin{origin}, m_direction{direction}
	{
	}
		
	// getters
	const point3& getOrigin() const { return m_origin; }
	const Vec3& getDirection() const { return m_direction; }

	// find location of a point along a ray when given t
	const point3 at(float t) const
	{
		return (m_origin + (t * m_direction)); // P(t) = A + tb
	}
};