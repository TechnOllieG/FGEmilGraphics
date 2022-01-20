#include <stdio.h>
#include "Library.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>
#include "MeshDefinitions.h"
#include "ShaderRegistry.h"
#include "Engine.h"

int main()
{
	engInit();

	spawnObject(MeshType::Quad, ShaderName::test);
	spawnObject(MeshType::Triangle, ShaderName::poop);

	while (engBeginFrame())
	{
		
	}

	engTerminate();
	return 0;
}