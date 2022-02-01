#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Library.h"
#include "Camera.h"
#include "Engine.h"

using namespace glm;

int main()
{
	engInit();

	ObjectHandle grass = spawnObject(vec3(0.f), quat(), vec3(50.f), Mesh::Quad, ShaderProgram::Test, Texture::Grass);
	ObjectHandle block = spawnObject(vec3(0.f, 1.5f, 0.f), quat(), vec3(1.f), Mesh::Cube, ShaderProgram::Test, Texture::Gravel);
	
	while (engLoop())
	{
		
	}
	
	return 0;
}