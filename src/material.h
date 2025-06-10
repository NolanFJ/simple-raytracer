#pragma once

#include "hittable.h"
#include "color.h"
#include "ray.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
private:
	color m_albedo{};

public:
	explicit Lambertian(const color& albedo);

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
};

class Metal : public Material
{
private:
	color m_albedo{};
	float m_fuzz{};

public:
	explicit Metal(const color& albedo, float fuzz);

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
};

class  Dielectric : public Material
{
private:
	// of material assuming surrounded by air
	float m_refractiveIndex{};

	// Schlick's approximation (assuming n1 is air)
	static float reflectance(float cos, float refractiveIndex);

public:
	explicit Dielectric(float refractiveIndex);

	bool scatter(const Ray& incident, const HitRecord& rec, color& attenuation, Ray& scattered) const override;
};