#pragma once

#include "ray.h"
#include "vec3.h"
#include "interval.h"

#include <memory>

class Material;

// details of where object gets hit
class HitRecord
{
private:
	point3 m_point{};
	Vec3 m_normal{};
	float m_t{};
	bool m_frontFace{};
	std::shared_ptr<Material> m_material{};

public:
	// getters
	point3 getPoint() const { return m_point; }
	Vec3 getNormal() const { return m_normal; }
	float getT() const { return m_t; }
	std::shared_ptr<Material> getMaterial() const { return m_material; }
	bool getFrontFace() const { return m_frontFace; }
	
	// setters
	void setPoint(const point3& point) { m_point = point; }
	void setT(float t) { m_t = t; }	
	void setMaterial(std::shared_ptr<Material> material) { m_material = std::move(material); }

	// always want the normal to point against the ray's direction
	void setFaceNormal(const Ray& ray, const Vec3& unitOutwardNormal)
	{
		m_frontFace = dot(ray.getDirection(), unitOutwardNormal) < 0.0;

		// redirect normal depending on where incident ray is
		m_normal = m_frontFace ? unitOutwardNormal : -unitOutwardNormal;
	}

};

// abstract base class for any object that can be hit by a ray
class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, const Interval& interval, HitRecord& rec) const = 0;
};