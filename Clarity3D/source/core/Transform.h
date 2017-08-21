#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
	explicit Transform() : scale(1) { }

	glm::vec3 forwards() {
		return glm::vec3(glm::mat4(rotation) * glm::vec4(0, 0, 1, 0));
	}

	glm::vec3 right() {
		return glm::vec3(glm::mat4(rotation) * glm::vec4(1, 0, 0, 0));
	}

	glm::vec3 up() {
		return glm::vec3(glm::mat4(rotation) * glm::vec4(0, 1, 0, 0));
	}

	void rotateAround(float angle, glm::vec3 axis) {
		rotation = glm::rotate(rotation, angle, axis);
	}


	glm::mat4 getModelMatrix() {
		glm::mat4 translate = glm::translate(position);
		return translate * glm::mat4(rotation) * glm::scale(scale);
	}

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
};