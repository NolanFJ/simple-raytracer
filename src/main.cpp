#include "vec3.h"
#include "color.h"

#include <iostream>
#include <fstream>

void render()
{
	const int width{1024};
	const int height{768};

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
			color pixelColor{(static_cast<float>(j) / (width - 1)), (static_cast<float>(i) / (height - 1)), 0};
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
