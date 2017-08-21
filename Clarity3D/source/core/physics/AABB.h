#pragma once
#include <glm/glm.hpp>

struct AABB {
	AABB() = default;
	AABB(glm::vec3 t_origin, glm::vec3 t_size) : origin(t_origin), size(t_size) {

	}

	glm::vec3 origin;
	glm::vec3 size;
};