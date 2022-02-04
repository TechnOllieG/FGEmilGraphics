#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

float clamp(float val, float min, float max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}