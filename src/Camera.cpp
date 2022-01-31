#include "Camera.h"

extern GLFWwindow* window;

mat4 Camera::getViewMatrix()
{
	vec3 forward = rotation * vec3(0.f, 0.f, 1.f);
	vec3 right = rotation * vec3(1.f, 0.f, 0.f);

	// Up-Down
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		position.y += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		position.y -= 0.05f;

	// Right-Left
	if (glfwGetKey(window, GLFW_KEY_D))
		position -= right * 0.05f;
	if (glfwGetKey(window, GLFW_KEY_A))
		position += right * 0.05f;

	// Forward-Backward
	if (glfwGetKey(window, GLFW_KEY_W))
		position += forward * 0.05f;
	if (glfwGetKey(window, GLFW_KEY_S))
		position -= forward * 0.05f;

	// Turn left-right
	if(glfwGetKey(window, GLFW_KEY_E))
	{
		quat turnDelta = angleAxis(-0.01f, vec3(0.f, 1.f, 0.f));
		rotation = turnDelta * rotation;
	}
	if(glfwGetKey(window, GLFW_KEY_Q))
	{
		quat turnDelta = angleAxis(0.01f, vec3(0.f, 1.f, 0.f));
		rotation = turnDelta * rotation;
	}

	vec3 center = position + forward - vec3(0.f, 2.f, 0.f);
	return glm::lookAt(position, center, vec3(0.f, 1.f, 0.f));
}
