#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.h"
#include "ShaderPrograms.h"
#include "Texture.h"

using namespace glm;

class Object
{
public:
	bool active = false;
	vec3 position = vec3(0.f);
	quat rotation = quat();
	vec3 scale = vec3(1.f);
	Mesh mesh = Mesh::MAX;
	ShaderProgram shaderProgram = ShaderProgram::MAX;
	Texture texture = Texture::MAX;
};