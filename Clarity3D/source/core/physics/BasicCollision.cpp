#include "BasicCollision.h"

namespace basic_collision {

	std::pair<bool, float> intersect_ray_aabb(Ray& ray, AABB& aabb) {
		float tmin = 0.f;
		float tmax = std::numeric_limits<float>::max();

		glm::vec3 aabb_max = aabb.origin + aabb.size;

		for (int i = 0; i < 3; i++) {
			if (abs(ray.direction[i]) < 0.0001) {
				// Ray is parallel to slab. No hit if origin not within slab
				if (ray.origin[i] < aabb.origin[i] || ray.origin[i] > aabb_max[i]) return std::make_pair(false, std::numeric_limits<float>::max());
			}

			else {
				float ood = 1.f / ray.direction[i];
				float t1 = (aabb.origin[i] - ray.origin[i]) * ood;
				float t2 = (aabb_max[i] - ray.origin[i]) * ood;

				if (t1 > t2) std::swap(t1, t2);

				tmin = std::max(tmin, t1);
				tmax = std::min(tmax, t2);

				if (tmin > tmax) return std::make_pair(false, std::numeric_limits<float>::max());
			}
		}

		return std::make_pair(true, tmin);
	}


}