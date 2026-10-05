#pragma once

#include "hittable.h"
#include "material.h"
#include "timer.h"

class Camera
{
private:
	int m_height{}; // height of image
	point3 m_lookFrom{}; // where the camera is located
	point3 m_lookAt{}; // point camera looks at
	Vec3 m_pixel00Location{}; // location of pixel at (0,0)
	Vec3 m_deltaU{}; // vector from pixel to pixel horizontally
	Vec3 m_deltaV{}; // vector from pixel to pixel vertically
	Vec3 m_w{}; // points backwards from the lookFrom point
	Vec3 m_u{}; // camera's x-axis
	Vec3 m_v{}; // camera's y-axis
	Vec3 m_vUp{}; // direction top of camera points to 
	Vec3 m_defocusU{}; // horizontal radius of defocus disk
	Vec3 m_defocusV{}; // vertical radius of defocus lens

	color rayColor(const Ray& ray, int depth, const Hittable& obj) const;
	
public:
	float aspectRatio{ 16.0f / 9 }; // ratio of width over height
	int width{ 1920 }; // image width
	int samplesPerPixel{ 500 }; // count of random samples for each pixel
	int maxDepth{ 50 }; // maximum number of ray bounces into the scene
	float verticalFOV{ 20 }; // how much the camera can see vertically
	float defocusAngle{ 0.6f }; // angle of rays through each pixel
	float focusDist{ 10.0f }; // distance from lookFrom to focus plane

	// initialize the camera/viewport values
	Camera();

	// write to ppm file and produce the image
	void render(const Hittable& world, const std::string& outputPath) const;

	// get rays of random samples per pixel
	Ray getRay(int i, int j) const;
};