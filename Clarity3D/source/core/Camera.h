#pragma once
#include <core/Transform.h>

class Camera {
public:
	Camera();

	glm::mat4 getViewMatrix();
	
	Transform transform;
	glm::mat4 projection;
private:

};