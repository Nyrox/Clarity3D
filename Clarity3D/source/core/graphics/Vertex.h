#pragma once
#include <glm/glm.hpp>

struct Vertex {
	Vertex() = default;
	explicit Vertex(glm::vec3 t_position, glm::vec3 t_normal = {}, glm::vec2 t_uv = {}, glm::vec3 t_tangent = {}) :
		position(t_position), normal(t_normal), uv(t_uv), tangent(t_tangent) {

	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
};