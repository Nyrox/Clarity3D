#pragma once
#include <core/physics/AABB.h>
#include <core/physics/Ray.h>

#include <algorithm>

namespace basic_collision {

	std::pair<bool, float> intersect_ray_aabb(Ray& ray, AABB& aabb);
}