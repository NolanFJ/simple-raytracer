#pragma once

#include "hittable.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
private:
	color m_albedo{};

public:
	Lambertian(const color& albedo)
		: m_albedo{ albedo }
	{
	}

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		auto scatterDir{ rec.getNormal() + randomUnit() };

		// make sure randomUnit was not the opposite of normal vec
		if (scatterDir.nearZero())
			scatterDir = rec.getNormal();

		scattered = Ray(rec.getPoint(), scatterDir);
		attenuation = m_albedo;

		return true;
	}
};

class Metal : public Material
{
private:
	color m_albedo{};
	float m_fuzz{};

public:
	Metal(const color& albedo, float fuzz)
		: m_albedo{ albedo }, m_fuzz{ fuzz }
	{
	}

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		auto reflectedDir{ reflect(incident.getDirection(), rec.getNormal()) };
		// fuzz out the reflections 
		auto fuzzVec{ m_fuzz * randomUnit()};

		// add fuzzVec to slightly change direction of reflected rays
		scattered = Ray(rec.getPoint(), reflectedDir + fuzzVec);
		attenuation = m_albedo;

		// make sure new reflected ray does not go back in the surface
		return (dot(scattered.getDirection(), rec.getNormal()) > 0);
	}
	
};