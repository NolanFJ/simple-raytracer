#pragma once

// details of where object gets hit
class HitRecord
{
private:
	point3 m_point{};
	Vec3 m_normal{};
	float m_t{};
	bool frontFace{};

public:
	// getters
	point3 getPoint() const { return m_point; }
	Vec3 getNormal() const { return m_normal; }
	float getT() const { return m_t; }
	
	// setters
	void setPoint(const point3& point) { m_point = point; }
	void setT(float t) { m_t = t; }	

	// always want the normal to point against the ray's direction
	void setFaceNormal(const Ray& ray, const Vec3& unitOutwardNormal)
	{
		frontFace = dot(ray.getDirection(), unitOutwardNormal) < 0.0;

		// redirect normal depending on where incident ray is
		m_normal = frontFace ? unitOutwardNormal : -unitOutwardNormal;
	}

};

// abstract base class for any object that can be hit by a ray
class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const = 0;
};