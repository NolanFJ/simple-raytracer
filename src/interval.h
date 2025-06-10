#pragma once

#include "math.h"

class Interval
{
private:
	float m_min{};
	float m_max{};

public:
	// default to an empty interval
	Interval()
		: m_min{ infinity }, m_max{ -infinity }
	{
	}
	// specific range
	Interval(float min, float max)
		: m_min{ min }, m_max{ max }
	{
	}

	// getters
	float getMin() const { return m_min; }
	float getMax() const { return m_max; }

	float size() const
	{
		return (m_max - m_min);
	}
	
	// inclusive bounds
	bool contains(float t) const
	{
		return (t >= m_min && t <= m_max);
	}

	// exclusive bounds
	bool surrounds(float t) const
	{
		return (t > m_min && t < m_max);
	}

	// ensure that color components remain in designated bounds
	float clamp(float x) const
	{
		if (x < m_min)
			return m_min;
		else if (x > m_max)
			return m_max;
		else
			return x;
	}

	static const Interval empty;
	static const Interval universe;
};