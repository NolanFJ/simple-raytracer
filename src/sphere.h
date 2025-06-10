#pragma once

#include "hittable.h"

#include <memory>

class Sphere : public Hittable
{
private:
	point3 m_center{};
	float m_radius{};
	std::shared_ptr<Material> m_material{};

public:
	Sphere(const point3& center, float radius, std::shared_ptr<Material> material);

	// return whether ray intersects through sphere or not
	// if it does, pass the hit information
	bool hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;
};	