#pragma once

#include "hittable.h"

#include <vector>

// stores a list of hittable objects
class HittableList : public Hittable
{
private:
	std::vector<std::shared_ptr<Hittable>> m_objects{};

public:
	HittableList()
	{
	}
	HittableList(std::shared_ptr<Hittable> object)
	{
		// add into array
		m_objects.push_back(object);
	}

	void add(std::shared_ptr<Hittable> object)
	{
		m_objects.push_back(object);
	}

	// clear the array
	void clear()
	{
		m_objects.clear();
	}

	bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override
	{
		HitRecord tempRec{};
		bool hit{};
		float closest{ tmax };

		// iterate through all objects
		for (const auto& object : m_objects)
		{
			if (object->hit(ray, tmin, closest, tempRec)) // check to see if they hit
			{
				hit = true;
				closest = tempRec.getT();
				rec = tempRec;
			}
		}
		return hit;
	}
};