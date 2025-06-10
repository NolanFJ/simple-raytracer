#include "material.h"

Lambertian::Lambertian(const color& albedo)
	: m_albedo{ albedo }
{
}

bool Lambertian::scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const
{
	auto scatterDir{ rec.getNormal() + randomUnit() };

	// make sure randomUnit was not the opposite of normal vec
	if (scatterDir.nearZero())
		scatterDir = rec.getNormal();

	scattered = Ray(rec.getPoint(), scatterDir);
	attenuation = m_albedo;

	return true;
}

Metal::Metal(const color& albedo, float fuzz)
	: m_albedo{ albedo }, m_fuzz{ fuzz }
{
}

bool Metal::scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const
{
	auto reflectedDir{ reflect(incident.getDirection(), rec.getNormal()) };
	// fuzz out the reflections 
	auto fuzzVec{ m_fuzz * randomUnit() };

	// add fuzzVec to slightly change direction of reflected rays
	scattered = Ray(rec.getPoint(), reflectedDir + fuzzVec);
	attenuation = m_albedo;

	// make sure new reflected ray does not go back in the surface
	return (dot(scattered.getDirection(), rec.getNormal()) > 0);
}

Dielectric::Dielectric(float refractiveIndex)
	: m_refractiveIndex{ refractiveIndex }
{
}

float Dielectric::reflectance(float cos, float refractiveIndex)
{
	float r0{ ((1 - refractiveIndex) / (1 + refractiveIndex)) * ((1 - refractiveIndex) / (1 + refractiveIndex)) };
	return float((r0 + (1 - r0) * std::pow((1 - cos), 5)));
}

bool Dielectric::scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const
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