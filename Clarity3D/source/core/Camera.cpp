#include "Camera.h"

Camera::Camera() {

}



glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(transform.position, transform.position + transform.forwards(), { 0, 1, 0 });
}