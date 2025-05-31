#pragma once

#include <iostream>
#include <cmath>
#include <memory>
#include <limits>

// useful math constants
constexpr float infinity{ std::numeric_limits<float>::infinity() };
constexpr float pi{ 3.1415926535897932385 };

// converting from degrees to radians
inline float degreesToRadians(float degrees)
{
	return (degrees * (pi / 180));
}


#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "camera.h"
