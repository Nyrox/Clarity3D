#pragma once
#include <glm/glm.hpp>

struct Ray {
	Ray() = default;
	explicit Ray(glm::vec3 t_origin, glm::vec3 t_dir) : origin(t_origin), direction(t_dir) {

	}

	glm::vec3 origin;
	glm::vec3 direction;
};