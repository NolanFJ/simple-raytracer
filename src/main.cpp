#include "math.h"	
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"

int main()
{
	// Objects to appear in the scene
	HittableList world{};

	world.add(std::make_shared<Sphere>(point3(0, 0, -1.0), 0.5));
	world.add(std::make_shared<Sphere>(point3(0, -100.5, -1.0), 100.0));

	Camera camera{400}; // pass the image width

	camera.render(world);

	return 0;
}
