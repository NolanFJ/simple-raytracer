#include "hittableList.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include "color.h"
#include "math.h"
#include "sphere.h"
#include "camera.h"

#include <memory>

// create the scene 
static HittableList scene()
{
	// Objects to appear in the scene
	HittableList world{};

	// floor
	auto materialGround{ std::make_shared<Lambertian>(color(0.5f, 0.5f, 0.5f)) };
	world.add(std::make_shared<Sphere>(point3(0.0f, -1000.0f, 0.0f), 1000, materialGround));

	for (int i{ -11 }; i < 11; ++i)
	{
		for (int j{ -11 }; j < 11; ++j)
		{
			auto chooseMaterial{ generateRandom() };
			point3 center{ i + 0.9f * generateRandom(), 0.2f, j + 0.9f * generateRandom() };

			if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9)
			{
				std::shared_ptr<Material> sphereMaterial{};

				// chance for diffuse material
				if (chooseMaterial < 0.8f)
				{
					auto albedo{ color::randomVec() * color::randomVec() };
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				// chance for metal material
				else if (chooseMaterial < 0.95f)
				{
					auto albedo{ color::randomVec(0.5f, 1.0f) };
					auto fuzz{ generateRandom(0.0f, 0.5f) };
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				// chance for glass material
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	// Large diffuse sphere
	auto material1{ std::make_shared<Lambertian>(color(0.4f, 0.2f, 0.1f)) };
	world.add(std::make_shared<Sphere>(point3(-4.0f, 1.0f, 0.0f), 1.0f, material1));

	// Large metal sphere
	auto material2{ std::make_shared<Metal>(color(0.7f, 0.6f, 0.5f), 0.0f) };
	world.add(std::make_shared<Sphere>(point3(4.0f, 1.0f, 0.0f), 1.0f, material2));

	// Large glass sphere
	auto material3{ std::make_shared<Dielectric>(1.5f) };
	world.add(std::make_shared<Sphere>(point3(0.0f, 1.0f, 0.0f), 1.0f, material3));

	return world;
}

int main()
{
	HittableList world{ scene() };

	Camera cam{};
	cam.render(world);

	return 0;
}
