#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Object
{
	vec3 position = vec3(0.f);
	quat rotation = quat(1.f, 0.f, 0.f, 0.f);
};