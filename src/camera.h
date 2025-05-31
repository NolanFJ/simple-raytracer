#pragma once

#include "hittable.h"
#include <fstream>

class Camera
{
private:
	int m_height{}; // height of image
	point3 m_cameraCenter{}; 
	Vec3 m_pixel00Location{}; // location of pixel at (0,0)
	Vec3 m_deltaU{}; // vector from pixel to pixel horizontally
	Vec3 m_deltaV{}; // vector from pixel to pixel vertically

	color rayColor(const Ray& ray, const Hittable& obj) const
	{
		HitRecord rec{};
		if (obj.hit(ray, Interval(0, infinity), rec))
		{
			return 0.5 * color(rec.getNormal().getX() + 1, rec.getNormal().getY() + 1, rec.getNormal().getZ() + 1);
		}

		// blendedValue = (1 - a) * startValue + a * endValue
		auto a{ 0.5 * (ray.getDirection().getY() + 1) };
		return ((1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0));
	}
	
public:
	const float aspectRatio{ 16.0 / 9 };
	const int width{};
	const float focalLength{ 1.0 };

	// initialize the camera/viewport values
	Camera(int w = 1024, float aspectR = (16.0 / 9))
		: width{ w }, aspectRatio { aspectR } 
	{
		m_height = static_cast<int>(width / aspectRatio);
		m_height = (m_height < 1) ? 1 : m_height; // make sure height is at least 1
		m_cameraCenter = point3(0, 0, 0);

		constexpr float viewportHeight{ 2.0 };
		float viewportWidth{ viewportHeight * (static_cast<float>(width) / m_height) };

		Vec3 viewportU{ viewportWidth, 0, 0 }; // V_u is vec from left to right edge
		Vec3 viewportV{ 0, -viewportHeight, 0 }; // V_v is vec from upper to lower edge

		m_deltaU = viewportU / width;
		m_deltaV = viewportV / m_height;

		//									brings to center			move left			move up
		auto viewportUpperLeft{ m_cameraCenter - Vec3(0, 0, focalLength) - (viewportU / 2) - (viewportV / 2) };
		// center of very top-left pixel
		m_pixel00Location = viewportUpperLeft + (0.5 * (m_deltaU + m_deltaV));
	}

	// write to ppm file and produce the image
	void render(const Hittable& world) const
	{
		std::ofstream outf{ "../../../Image.ppm", std::ios::binary };
		// check if file cannot open
		if (!outf)
		{
			std::cerr << "Image file could not be opened.\n";
			exit(1);
		}

		// P6 expects binary format, print the dimensions of the image
		outf << "P6\n" << width << " " << m_height << "\n255\n";

		// write to each pixel
		for (int i{}; i < m_height; ++i)
		{
			// progress indicator
			std::clog << "\rScanlines remaining: " << (m_height - i) << " " << std::flush;

			for (int j{}; j < width; ++j)
			{
				auto pixelCenter{ m_pixel00Location + (i * m_deltaV) + (j * m_deltaU) };
				auto rayDirection{ unit(pixelCenter - m_cameraCenter) };

				// starts at camera center then directed towards rayDirection
				Ray ray{ m_cameraCenter, rayDirection };

				// get color of each ray
				color pixelColor{ rayColor(ray, world) };

				writeColor(outf, pixelColor);
			}
		}
		std::clog << "\rDone.                  \n";
	}
};