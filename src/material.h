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

class  Dielectric : public Material
{
private:
	// of material assuming surrounded by air
	float m_refractiveIndex{};

	// Schlick's approximation (assuming n1 is air)
	static float reflectance(float cos, float refractiveIndex)
	{
		float r0{ ((1 - refractiveIndex) / (1 + refractiveIndex)) * ((1 - refractiveIndex) / (1 + refractiveIndex)) };
		return (r0 + (1 - r0) * std::pow((1 - cos), 5));
	}

public:
	Dielectric(float refractiveIndex)
		: m_refractiveIndex{ refractiveIndex }
	{
	}

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		attenuation = color(1.0f, 1.0f, 1.0f); // absorbs no color
		float index{ rec.getFrontFace() ? (1.0f / m_refractiveIndex) : (m_refractiveIndex) }; // ratio of refractive index (assuming n1 is air)
		Vec3 unitDir{ unit(incident.getDirection()) };

		// useful trig functions
		float cos{ dot(-unitDir, rec.getNormal()) };
		float sin{ std::sqrt(1 - (cos * cos)) };

		// if TIR happens reflect, and also implement Schlick approximation
		if (((index * sin) > 1.0f) || (reflectance(cos, index) > generateRandom()))
		{
			Vec3 reflected{ reflect(incident.getDirection(), rec.getNormal()) };
			scattered = Ray(rec.getPoint(), reflected);
		}
		// otherwise refract
		else
		{
			Vec3 refracted{ refract(index, unitDir, rec.getNormal()) };
			scattered = Ray(rec.getPoint(), refracted);
		}

		return true;
	}
};