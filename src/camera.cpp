#include "camera.h"
#include "color.h"
#include "ray.h"
#include "math.h"

#include <fstream>
#include <cmath>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>

color Camera::rayColor(const Ray& ray, int depth, const Hittable& obj) const
{
	// protect against long recursion
	if (depth <= 0)
		return color(0, 0, 0);

	HitRecord rec{};

	if (obj.hit(ray, Interval(1e-3f, infinity), rec))
	{
		Ray scattered{};
		color attenuation{};
		// if the ray scatters (100% prob currently)
		if (rec.getMaterial()->scatter(ray, rec, attenuation, scattered))
		{
			return attenuation * rayColor(scattered, depth - 1, obj);
		}
		return color(0, 0, 0);
	}

	// blendedValue = (1 - a) * startValue + a * endValue
	auto a{ 0.5f * (ray.getDirection().getY() + 1.0f) };
	return ((1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f));
}

Camera::Camera()
{
	m_height = static_cast<int>(width / aspectRatio);
	m_height = (m_height < 1) ? 1 : m_height; // make sure height is at least 1
	m_lookFrom = point3(13.0f, 2.0f, 3.0f);
	m_lookAt = point3(0.0f, 0.0f, 0.0f);
	m_vUp = Vec3(0.0f, 1.0f, 0.0f);

	// camera coordinate system (orthonormal basis)
	m_w = unit(m_lookFrom - m_lookAt);
	m_u = unit(cross(m_vUp, m_w));
	m_v = cross(m_w, m_u);

	auto h{ std::tan(degreesToRadians(verticalFOV) / 2) };

	float viewportHeight{ 2 * h * focusDist };
	float viewportWidth{ viewportHeight * (static_cast<float>(width) / m_height) };

	Vec3 viewportU{ viewportWidth * m_u }; // vec from left to right edge
	Vec3 viewportV{ viewportHeight * -m_v }; // vec from upper to lower edge

	m_deltaU = viewportU / float(width);
	m_deltaV = viewportV / float(m_height);

	//									brings to center			move left			move up
	auto viewportUpperLeft{ m_lookFrom - (focusDist * m_w) - (viewportU / 2) - (viewportV / 2) };
	// center of very top-left pixel
	m_pixel00Location = viewportUpperLeft + (0.5 * (m_deltaU + m_deltaV));

	// calculate the camera defocus disk basis vectors
	auto defocusRadius{ focusDist * std::tan(degreesToRadians(defocusAngle / 2)) };
	m_defocusU = m_u * defocusRadius;
	m_defocusV = m_v * defocusRadius;
}

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

void Camera::render(const Hittable& world, const std::string& outputPath) const
{
    std::ofstream outf{ outputPath, std::ios::binary };
    if (!outf)
    {
        std::cerr << "Could not open output file: " << outputPath << '\n';
        exit(1);
    }

    outf << "P6\n" << width << " " << m_height << "\n255\n";

    Timer t;

    std::vector<color> pixels(static_cast<std::size_t>(width) * m_height);
    std::atomic<int> nextRow{ 0 };
    std::atomic<int> rowsDone{ 0 };

    // each thread grabs the next unrendered row until none are left
    auto worker = [&]()
    {
        while (true)
        {
            int i{ nextRow.fetch_add(1) };
            if (i >= m_height)
                break;

            for (int j{}; j < width; ++j)
            {
                color pixelColor{};
                for (int k{}; k < samplesPerPixel; ++k)
                    pixelColor += rayColor(getRay(i, j), maxDepth, world);

                pixels[static_cast<std::size_t>(i) * width + j] = pixelColor / float(samplesPerPixel);
            }
            ++rowsDone;
        }
    };

    unsigned int threadCount{ std::thread::hardware_concurrency() };
    if (threadCount == 0)
        threadCount = 1;

    std::vector<std::thread> threads;
    for (unsigned int n{}; n < threadCount; ++n)
        threads.emplace_back(worker);

    // progress indicator on the main thread only, so output doesn't interleave
    while (rowsDone < m_height)
    {
        std::clog << "\rRendering...  " << std::round(rowsDone / float(m_height) * 100) << "%  " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto& th : threads)
        th.join();

    // write sequentially so the image comes out in the right order
    for (const auto& p : pixels)
        writeColor(outf, p);

    std::clog << "\rDone.                  \n";
    std::cout << "Time taken: " << t.elapsed() << " seconds.\n";
	std::cout << "Saved to " << std::filesystem::absolute(outputPath) << '\n';
}

Ray Camera::getRay(int i, int j) const
{
	// generate a random point on defocus disk to shoot ray from
	auto randomDisk{ randomUnitDisk() };
	auto defocusOffset{ (randomDisk.getX() * m_defocusU) + (randomDisk.getY() * m_defocusV) };

	// no defocus blur for negative angles
	auto origin{ (defocusAngle <= 0) ? m_lookFrom : m_lookFrom + defocusOffset };

	// target point on the focus plane
	auto pixelTarget = m_pixel00Location + ((j + generateRandom()) * m_deltaU) + ((i + generateRandom()) * m_deltaV);
	auto rayDirection{ unit(pixelTarget - origin) };

	return Ray(origin, rayDirection);
}