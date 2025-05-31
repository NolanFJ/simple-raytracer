#pragma once

#include "vec3.h"

using color = Vec3;

void writeColor(std::ostream& out, const color& pixelColor)
{
	auto r{pixelColor.getX()};
	auto g{pixelColor.getY()};
	auto b{pixelColor.getZ()};

	// scale from 1 to 255
	// using P6 so needs binary format
	const uint8_t rByte{static_cast<uint8_t>(255.999 * r)};
	const uint8_t gByte{static_cast<uint8_t>(255.999 * g)};
	const uint8_t bByte{static_cast<uint8_t>(255.999 * b)};

	// write out the colors
	out.write(reinterpret_cast<const char*>(&rByte), 1);
	out.write(reinterpret_cast<const char*>(&gByte), 1);
	out.write(reinterpret_cast<const char*>(&bByte), 1);
	
}