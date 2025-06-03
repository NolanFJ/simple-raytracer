#pragma once

#include "hittable.h"

class Sphere : public Hittable
{
private:
	point3 m_center{};
	float m_radius{};
	std::shared_ptr<Material> m_material{};

public:
	Sphere(const point3& center, float radius, std::shared_ptr<Material> material)
		: m_center{ center }, m_radius{ static_cast<float>(std::fmax(0, radius)) }, m_material{ std::move(material) }
	{ //						make sure radius cannot be negative
	}

	bool hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override
	{
		auto centerMinusOrigin{ m_center - ray.getOrigin() };

		// values for quadratic eqn
		auto a{ ray.getDirection().lengthSquared() };
		auto h{ dot(ray.getDirection(), centerMinusOrigin) };
		auto c{ centerMinusOrigin.lengthSquared() - (m_radius * m_radius) };
		auto discriminant{ (h * h) - (a * c) };

		if (discriminant < 0)
			return false; // no hit
		
		auto sqrtd{ std::sqrt(discriminant) };
		auto root{ (h - std::sqrt(discriminant)) / a }; // smaller root
		
		if (root <= interval.getMin() || root >= interval.getMax()) // if smaller root is invalid
		{
			root = h + std::sqrt(discriminant); // try larger root
			if (root <= interval.getMin() || root >= interval.getMax())
				return false; // no valid roots
		}

		// set the details of how sphere is hit
		rec.setT(root);
		rec.setPoint(ray.at(root));
		rec.setFaceNormal(ray, unit(rec.getPoint() - m_center));
		rec.setMaterial(m_material);

		return true;
	}
};	