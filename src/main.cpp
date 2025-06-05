#include "math.h"	
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"
#include "material.h"

int main()
{
	// Objects to appear in the scene
	HittableList world{};

	// set material of each object
	auto materialGround{ std::make_shared<Lambertian>(color(0.8f, 0.8f, 0.0f)) };
	auto materialCenter{ std::make_shared<Lambertian>(color(0.1f, 0.2f, 0.5f)) };
	auto materialLeft{ std::make_shared<Dielectric>(1.50f) }; // glass
	auto materialInsideLeft{ std::make_shared<Dielectric>(1.00f / 1.50f) }; // make glass sphere hollow
	auto materialRight{ std::make_shared<Metal>(color(0.83f, 0.69f, 0.22f), 0.05f) };// Gold

	// add each object into the scene
	world.add(std::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.add(std::make_shared<Sphere>(point3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
	world.add(std::make_shared<Sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	world.add(std::make_shared<Sphere>(point3(-1.0f, 0.0f, -1.0f), 0.4f, materialInsideLeft));
	world.add(std::make_shared<Sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

	Camera camera{};

	camera.render(world);

	return 0;
}
