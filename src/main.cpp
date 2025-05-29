#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>
#include <fstream>

// returns color for given scene ray
color rayColor(const Ray& ray)
{
	// blendedValue = (1 - a) * startValue + a * endValue
	auto a{0.5 * (ray.getDirection().getY() + 1)};
	return ((1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0));
}

void render()
{
	constexpr float aspectRatio{16.0 / 9};

	// image dimensions
	constexpr int width{1024};
	int height{static_cast<int>(width / aspectRatio)};
	height = (height < 1) ? 1 : height; // make sure height is at least 1

	// distance between viewport and camera
	float focalLength{1.0};

	point3 cameraCenter{0, 0, 0};

	// viewport dim
	constexpr float viewportHeight{2.0};
	float viewportWidth{viewportHeight * (static_cast<float>(width) / height)};

	Vec3 viewportU{viewportWidth, 0, 0}; // V_u is vec from left to right edge
	Vec3 viewportV{0, -viewportHeight, 0}; // V_v is vec from upper to lower edge

	// vectors from pixel to pixel
	Vec3 deltaU{viewportU / width};
	Vec3 deltaV{viewportV / height};

	//									brings to center			move left			move up
	auto viewportUpperLeft{cameraCenter - Vec3(0, 0, focalLength) - (viewportU / 2) - (viewportV / 2)};

	// center of very top-left pixel
	auto pixel00Location{viewportUpperLeft + (0.5 * (deltaU + deltaV))};

	std::ofstream outf{"../../../Image.ppm", std::ios::binary};
	// check if file cannot open
	if (!outf)
	{
		std::cerr << "Image file could not be opened.\n";
		exit(1);
	}
		
	// P6 expects binary format, print the dimensions of the image
	outf << "P6\n" << width << " " << height << "\n255\n";

	// write to each pixel
	for (int i{}; i < height; ++i)
	{
		// progress indicator
		std::clog << "\rScanlines remaining: " << (height - 1) << " " << std::flush;

		for (int j{}; j < width; ++j)
		{
			auto pixelCenter{pixel00Location + (i * deltaV) + (j * deltaU)};
			auto rayDirection{unit(pixelCenter - cameraCenter)};

			// starts at camera center then directed towards rayDirection
			Ray ray{cameraCenter, rayDirection};

			// get color of each ray
			color pixelColor{rayColor(ray)};

			writeColor(outf, pixelColor);	
		}
	}
	std::clog << "\rDone.                  \n";
}

int main()
{
	render();

	return 0;
}
