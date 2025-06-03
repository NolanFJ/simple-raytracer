#pragma once

#include <iostream>
#include <cmath>
#include <memory>
#include <limits>
#include <random>

// useful math constants
constexpr float infinity{ std::numeric_limits<float>::infinity() };
constexpr float pi{ 3.1415926535897932385 };

// converting from degrees to radians
inline float degreesToRadians(float degrees)
{
	return (degrees * (pi / 180));
}

// return a random number from 0f - 1f using Mersenne Twister
inline float generateRandom()
{
	static std::mt19937 engine{ std::random_device {}() };
	static std::uniform_real_distribution<float> random(0.0f, 1.0f);

	return random(engine);

}

// return a random number from min - max using Mersenne Twister
inline float generateRandom(float min, float max) 
{
	return min + (max - min) * generateRandom();

}


#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "camera.h"
#include "material.h"
