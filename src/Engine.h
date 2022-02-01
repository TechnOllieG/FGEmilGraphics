#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "ShaderPrograms.h"
#include "Texture.h"
#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMAGE_CHANNEL_AMOUNT 4
#define MAX_OBJECT_AMOUNT 128
#define INVALID_OBJECT_HANDLE -1

GLFWwindow* window;
Camera camera = Camera(vec3(-10.f, 1.f, 5.f));

int windowWidth = 800.f;
int windowHeight = 600.f;
float time = 0.f;

GLuint meshes[(int)Mesh::MAX];
bool meshesLoaded = false;
GLuint shaderPrograms[(int)ShaderProgram::MAX];
bool shaderProgramsLoaded = false;
GLuint textures[(int)Texture::MAX];
bool texturesLoaded = false;

Object objects[MAX_OBJECT_AMOUNT];

typedef int ObjectHandle;

void handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void flipTexture(stbi_uc* data, int width, int height, int components)
{
	int lineSize = width * components;
	int totalSize = width * height * components;
	stbi_uc* buffer = (stbi_uc*)malloc(totalSize);
	memcpy(buffer, data, totalSize);

	for (int y = 0; y < height; ++y)
	{
		int flippedY = height - 1 - y;
		memcpy(data + lineSize * y, buffer + lineSize * flippedY, lineSize);
	}

	free(buffer);
}

GLuint loadMesh(const void* data, unsigned int size, const void* elementData, unsigned int elementSize)
{
	// Setup the VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Setting up the vertex buffer! (VBO)
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	// Bind buffers to positon and color attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	// Create Element Buffer
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize, elementData, GL_STATIC_DRAW);

	return vao;
}

void loadMeshes()
{
	for (int i = 0; i < (int)Mesh::MAX; i++)
	{
		meshes[i] = loadMesh(mesh_Data[i], mesh_Data_Size[i], mesh_Indices[i], mesh_Indices_Size[i]);
	}
	meshesLoaded = true;
}

GLuint getMesh(Mesh mesh)
{
	if (!meshesLoaded)
		loadMeshes();

	return meshes[(int)mesh];
}

GLuint loadShader(GLenum type, const char* path)
{
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

	return shader;
}

GLuint loadProgram(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	return program;
}

void loadPrograms()
{
	for (int i = 0; i < (int) ShaderProgram::MAX; i++)
	{
		shaderPrograms[i] = loadProgram(shader_Paths[i * 2], shader_Paths[i * 2 + 1]);
	}
	shaderProgramsLoaded = true;
}

GLuint getProgram(ShaderProgram program)
{
	if (!shaderProgramsLoaded)
		loadPrograms();

	return shaderPrograms[(int)program];
}

GLuint loadTexture(const char* path)
{
	// Load texture from file
	int texWidth;
	int texHeight;
	int texComponents;
	stbi_uc* texPixels = stbi_load(path, &texWidth, &texHeight, &texComponents, IMAGE_CHANNEL_AMOUNT);

	flipTexture(texPixels, texWidth, texHeight, IMAGE_CHANNEL_AMOUNT);

	// Upload to OpenGL
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(texPixels);

	return texture;
}

void loadTextures()
{
	for (int i = 0; i < (int)Texture::MAX; i++)
	{
		textures[i] = loadTexture(texture_Paths[i]);
	}
	texturesLoaded = true;
}

GLuint getTexture(Texture texture)
{
	if (!texturesLoaded)
		loadTextures();

	return textures[(int)texture];
}

ObjectHandle spawnObject(vec3 position, quat rotation, vec3 scale, Mesh mesh, ShaderProgram shaderProgram, Texture texture)
{
	for (int i = 0; i < MAX_OBJECT_AMOUNT; i++)
	{
		if (objects[i].active)
			continue;

		objects[i].position = position;
		objects[i].rotation = rotation;
		objects[i].scale = scale;
		objects[i].mesh = mesh;
		objects[i].shaderProgram = shaderProgram;
		objects[i].texture = texture;
		objects[i].active = true;
		return i;
	}

	return INVALID_OBJECT_HANDLE;
}

Object* getObject(ObjectHandle handle)
{
	if(handle >= 0 && handle < MAX_OBJECT_AMOUNT && objects[handle].active)
		return &objects[handle];

	return nullptr;
}

void destroyObject(ObjectHandle handle)
{
	if (handle >= 0 && handle < MAX_OBJECT_AMOUNT && objects[handle].active)
		objects[handle].active = false;
}

void engInit()
{
	glfwInit();

	window = glfwCreateWindow(800, 600, "Graphics Course", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetWindowSizeCallback(window, handleWindowResize);

	loadMeshes();
	loadTextures();
	loadPrograms();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

bool engLoop()
{
	glfwSwapBuffers(window);

	glfwPollEvents();
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	time += 0.01f;

	vec3 directionalLight = normalize(vec3(2.f, -5.f, -1.f));

	float aspect = (float)windowWidth / (float)windowHeight;
	mat4 projection = perspective(radians(90.f), aspect, 0.1f, 100.f);
	mat4 view = camera.getViewMatrix();

	for (int i = 0; i < MAX_OBJECT_AMOUNT; i++)
	{
		if (!objects[i].active)
			continue;

		Object obj = objects[i];
		GLuint shader = getProgram(obj.shaderProgram);
		glUseProgram(shader);
		glBindVertexArray(getMesh(obj.mesh));

		// Calculate model matrix
		mat4 model = translate(mat4(1.f), obj.position);
		model = rotate(mat4(1.f), -pi<float>() / 2.f, vec3(1.f, 0.f, 0.f));
		model = scale(model, obj.scale);

		// Set uniforms
		glUniformMatrix4fv(glGetUniformLocation(shader, "u_Projection"), 1, false, (GLfloat*)&projection);
		glUniform1f(glGetUniformLocation(shader, "u_Time"), time);
		glUniformMatrix4fv(glGetUniformLocation(shader, "u_View"), 1, false, (GLfloat*)&view);
		glUniform3fv(glGetUniformLocation(shader, "u_EyePosition"), 1, (float*)&camera.position);
		glUniform3fv(glGetUniformLocation(shader, "u_DirLight"), 1, (float*)&directionalLight);
		glUniformMatrix4fv(glGetUniformLocation(shader, "u_Model"), 1, false, (GLfloat*)&model);
		glBindTexture(GL_TEXTURE_2D, getTexture(obj.texture));

		// Render
		glDrawElements(GL_TRIANGLES, mesh_Indices_Size[(int)obj.mesh], GL_UNSIGNED_INT, nullptr);
	}

	//// Render grass
	//{
	//	mat4 model;// = translate(mat4(1.f), vec3(0.f, 0.f, -1.f));
	//	model = rotate(mat4(1.f), -pi<float>() / 2.f, vec3(1.f, 0.f, 0.f));
	//	model = scale(model, vec3(50.f));

	//	glBindVertexArray(getMesh(Mesh::Quad));
	//	glBindTexture(GL_TEXTURE_2D, getTexture(Texture::Grass));
	//	glUniformMatrix4fv(u_Model, 1, false, (GLfloat*)&model);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//}

	//// Render cube
	//{
	//	mat4 model;// = translate(mat4(1.f), vec3(0.f, 0.f, -1.f));
	//	model = translate(mat4(1.f), vec3(0.f, 1.5f, 0.f)) * rotate(mat4(1.f), time, vec3(2.f, 1.f, -3.2f));

	//	glBindVertexArray(getMesh(Mesh::Cube));
	//	glBindTexture(GL_TEXTURE_2D, getTexture(Texture::Gravel));
	//	glUniformMatrix4fv(u_Model, 1, false, (GLfloat*)&model);
	//	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
	//}

	if (glfwWindowShouldClose(window))
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	return true;
}