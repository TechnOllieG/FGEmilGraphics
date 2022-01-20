#include <stdio.h>
#include "Library.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>
#include "MeshDefinitions.h"

#define MAX_MESH_COUNT 1024
#define MAX_SHADER_COUNT 512
#define MAX_SHADER_PROGRAM_COUNT 256
#define MAX_OBJECT_COUNT 2048

Mesh loadedMeshes[MAX_MESH_COUNT];
GLuint loadedVAOs[MAX_MESH_COUNT];
int loadedMeshesIndicesCount[MAX_MESH_COUNT];

const char* loadedShaderPaths[MAX_SHADER_COUNT];
GLuint loadedShaders[MAX_SHADER_COUNT];

ShaderProgramSource loadedShaderProgramSources[MAX_SHADER_PROGRAM_COUNT];
GLint loadedShaderPrograms[MAX_SHADER_PROGRAM_COUNT];

Object objects[MAX_OBJECT_COUNT];

int getLoadedMeshIndex(Mesh mesh)
{
	for (int i = 0; i < MAX_MESH_COUNT; i++)
	{
		if (!loadedMeshes[i].isValid)
			return -1;

		if (loadedMeshes[i] == mesh)
			return i;

		return -1;
	}
}

int getLoadedShaderPathIndex(const char* path)
{
	for (int i = 0; i < MAX_SHADER_COUNT; i++)
	{
		if (loadedShaderPaths[i] == nullptr)
			return -1;

		if (loadedShaderPaths[i] == path)
			return i;

		return -1;
	}
}

int getLoadedShaderProgramSourceIndex(ShaderProgramSource source)
{
	for (int i = 0; i < MAX_SHADER_PROGRAM_COUNT; i++)
	{
		if (!loadedShaderProgramSources[i].isValid)
			return -1;

		if (loadedShaderProgramSources[i] == source)
			return i;

		return -1;
	}
}

void handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int loadMesh(Mesh meshData)
{
	int index = getLoadedMeshIndex(meshData);
	if (index != -1)
	{
		return loadedVAOs[index];
	}

	// Setup the VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Setting up the vertex buffer! (VBO)
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, meshData.dataSize, meshData.data, GL_STATIC_DRAW);

	// Bind buffers to positon and color attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	// Create Element Buffer
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.elementDataSize, meshData.elementData, GL_STATIC_DRAW);

	for (int i = 0; i < MAX_MESH_COUNT; i++)
	{
		if (loadedMeshes[i].isValid)
		{
			if (i == MAX_MESH_COUNT - 1)
			{
				printf("Mesh array is full");
				break;
			}
			continue;
		}

		loadedMeshes[i] = meshData;
		loadedVAOs[i] = vao;
		loadedMeshesIndicesCount[i] = meshData.elementDataSize;
		return i;
	}
}

int loadShader(GLenum type, const char* path)
{
	int index = getLoadedShaderPathIndex(path);
	if (index != -1)
	{
		return loadedShaders[index];
	}

	// Read source file !
	FILE* file = nullptr;
	fopen_s(&file, path, "rb");
	if (!file)
	{
		printf("Failed to load shader '%s'\n", path);
		return 0;
	}

	// Fetch file size
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read
	char* fileBuffer = (char*)malloc(fileSize);
	fileSize = fread(fileBuffer, 1, fileSize, file);

	fclose(file);

	static char infoBuffer[1024];

	// Shaders!
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &fileBuffer, &fileSize);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus)
	{
		glGetShaderInfoLog(shader, 1024, nullptr, infoBuffer);
		printf("COMPILE ERROR:\n%s\n", infoBuffer);
	}

	for (int i = 0; i < MAX_SHADER_COUNT; i++)
	{
		if (loadedShaderPaths[i] != nullptr)
		{
			if (i == MAX_SHADER_COUNT - 1)
			{
				printf("Shader array is full");
				break;
			}

			continue;
		}

		loadedShaderPaths[i] = path;
		loadedShaders[i] = shader;
		return i;
	}
}

int loadProgram(ShaderProgramSource source)
{
	int index = getLoadedShaderProgramSourceIndex(source);
	if (index != -1)
	{
		return loadedShaderPrograms[index];
	}

	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, source.vertexPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, source.fragmentPath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	for (int i = 0; i < MAX_SHADER_PROGRAM_COUNT; i++)
	{
		if (loadedShaderProgramSources[i].isValid)
		{
			if (i == MAX_SHADER_PROGRAM_COUNT - 1)
			{
				printf("Shader program array is full");
				break;
			}

			continue;
		}

		loadedShaderProgramSources[i] = source;
		loadedShaderPrograms[i] = program;
		return i;
	}
}

int spawnObject(GLuint vaoIndex, GLuint shaderProgramIndex)
{
	for (int i = 0; i < MAX_OBJECT_COUNT; i++)
	{
		if (objects[i].isValid)
			continue;

		objects[i].vaoIndex = vaoIndex;
		objects[i].shaderProgramIndex = shaderProgramIndex;
		objects[i].isValid = true;
		return i;
	}

	printf("Can't spawn object, object array is full");
	return -1;
}

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics Course", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetWindowSizeCallback(window, handleWindowResize);

	spawnObject(loadMesh(Mesh((void*)quad_Data, sizeof(quad_Data), (void*)quad_Index_Data, sizeof(quad_Index_Data))),
		loadProgram(ShaderProgramSource("shaders/test.vert", "shaders/test.frag")));

	spawnObject(loadMesh(Mesh((void*)triangle_Data, sizeof(triangle_Data), (void*)triangle_Index_Data, sizeof(triangle_Index_Data))),
		loadProgram(ShaderProgramSource("shaders/test.vert", "shaders/poop.frag")));

	float time = 0.f;

	while (!glfwWindowShouldClose(window))
	{
		time += 0.01f;

		glfwPollEvents();

		glClearColor(0.5f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < MAX_OBJECT_COUNT; i++)
		{
			if (!objects[i].isValid)
				break;

			GLuint program = loadedShaderPrograms[objects[i].shaderProgramIndex];
			GLuint vao = loadedVAOs[objects[i].vaoIndex];
			unsigned int elementCount = loadedMeshesIndicesCount[objects[i].vaoIndex];

			glUseProgram(program);

			glUniform1f(glGetUniformLocation(program, "u_Time"), time);
			glUniform2f(glGetUniformLocation(program, "u_Offset"), objects[i].position.x, objects[i].position.y);

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}