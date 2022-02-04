#include "Camera.h"

extern GLFWwindow* window;

mat4 Camera::getViewMatrix()
{
	vec3 forward = rotation * vec3(0.f, 0.f, 1.f);
	vec3 right = rotation * vec3(1.f, 0.f, 0.f);

	vec3 center = position + forward;
	return glm::lookAt(position, center, vec3(0.f, 1.f, 0.f));
}
