#pragma once

#include "vec3.h"
#include "interval.h"

using color = Vec3;

void writeColor(std::ostream& out, const color& pixelColor)
{
	auto r{pixelColor.getX()};
	auto g{pixelColor.getY()};
	auto b{pixelColor.getZ()};

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