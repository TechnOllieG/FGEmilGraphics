#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

struct Camera
{
	Camera() {}
	Camera(vec3 in_position)
	{
		position = in_position;
	}

	vec3 position = vec3(0.f);
	quat rotation = quat(1.f, 0.f, 0.f, 0.f);

	mat4 getViewMatrix();
};