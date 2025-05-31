#pragma once

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

	static const Interval empty;
	static const Interval universe;
};

// Initialize static const values
const Interval Interval::empty{ Interval(infinity, -infinity) };
const Interval Interval::universe{ Interval(-infinity, infinity) };