#pragma once

#include "vec3.h"
#include "interval.h"

using color = Vec3;

// convert from linear space to gamma 2 space (my image viewer is expecting gamma space)
inline float linearToGamma(float linear)
{
	return (linear > 0) ? std::sqrt(linear) : 0;
}

void writeColor(std::ostream& out, const color& pixelColor)
{
	// convert to gamma space values
	auto r{linearToGamma(pixelColor.getX())};
	auto g{linearToGamma(pixelColor.getY())};
	auto b{linearToGamma(pixelColor.getZ())};

	// scale from 1 to 255
	// using P6 so needs binary format
	static const Interval intensity{ 0.000f, 0.999f };
	const uint8_t rByte{ static_cast<uint8_t>(256 * intensity.clamp(r)) };
	const uint8_t gByte{ static_cast<uint8_t>(256 * intensity.clamp(g)) };
	const uint8_t bByte{ static_cast<uint8_t>(256 * intensity.clamp(b)) };

	// write out the colors
	out.write(reinterpret_cast<const char*>(&rByte), 1);
	out.write(reinterpret_cast<const char*>(&gByte), 1);
	out.write(reinterpret_cast<const char*>(&bByte), 1);
}