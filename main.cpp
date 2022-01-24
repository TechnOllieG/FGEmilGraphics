#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>
#include "Library.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Matrix
{
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;

	Matrix operator*(const Matrix& rhs) const
	{
		Matrix result;

		// m11 = Vector3.Dot(this.col(1) rhs.row(1))
		result.m11 = m11 * rhs.m11 + m21 * rhs.m12 + m31 * rhs.m13;
		// m12 = Vector3.Dot(this.col(2) rhs.row(1))
		result.m12 = m12 * rhs.m11 + m22 * rhs.m12 + m32 * rhs.m13;
		// m13 = Vector3.Dot(this.col(3) rhs.row(1))
		result.m13 = m13 * rhs.m11 + m23 * rhs.m12 + m33 * rhs.m13;

		// m11 = Vector3.Dot(this.col(1) rhs.row(2))
		result.m21 = m11 * rhs.m21 + m21 * rhs.m22 + m31 * rhs.m23;
		// m12 = Vector3.Dot(this.col(2) rhs.row(2))
		result.m22 = m12 * rhs.m21 + m22 * rhs.m22 + m32 * rhs.m23;
		// m13 = Vector3.Dot(this.col(3) rhs.row(2))
		result.m23 = m13 * rhs.m21 + m23 * rhs.m22 + m33 * rhs.m23;

		// m11 = Vector3.Dot(this.col(1) rhs.row(3))
		result.m31 = m11 * rhs.m31 + m21 * rhs.m32 + m31 * rhs.m33;
		// m12 = Vector3.Dot(this.col(2) rhs.row(3))
		result.m32 = m12 * rhs.m31 + m22 * rhs.m32 + m32 * rhs.m33;
		// m13 = Vector3.Dot(this.col(3) rhs.row(3))
		result.m33 = m13 * rhs.m31 + m23 * rhs.m32 + m33 * rhs.m33;

		return result;
	}
};

const float triangleA_Data[] =
{
	// position		// color			// uv
	-0.5f, -0.5f,	1.f, 0.f, 0.f,		0.f, 0.f,
	0.5f, -0.5f,	0.f, 1.f, 0.f,		1.f, 0.f,
	0.5f, 0.5f,		0.f, 0.f, 1.f,		1.f, 1.f,
	-0.5f, 0.5f,	1.f, 1.f, 0.f,		0.f, 1.f
};

const unsigned int triangleA_Index_Data[] =
{
	0, 1, 2,
	0, 2, 3
};

const float triangleB_Data[] =
{
	0.2f, 0.5f,		0.f, 0.5f, 0.5f,
	-0.3f, 0.4f,	0.5f, 0.5f, 0.5f,
	0.f, -0.1f,		0.f, 0.f, 0.8f
};

const unsigned int triangleB_Index_Data[] =
{
	0, 1, 2,
};

int windowWidth = 800.f;
int windowHeight = 600.f;

void handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
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
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	// Create Element Buffer
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize, elementData, GL_STATIC_DRAW);

	return vao;
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

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics Course", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetWindowSizeCallback(window, handleWindowResize);

	// Load meshes
	GLuint triangleA = loadMesh(triangleA_Data, sizeof(triangleA_Data), triangleA_Index_Data, sizeof(triangleA_Index_Data));
	GLuint triangleB = loadMesh(triangleB_Data, sizeof(triangleB_Data), triangleB_Index_Data, sizeof(triangleB_Index_Data));

	int texWidth;
	int texHeight;
	int texComponents;
	stbi_uc* texPixels = stbi_load("gravel.jpg", &texWidth, &texHeight, &texComponents, 3);

	GLuint programA = loadProgram("shaders/test.vert", "shaders/test.frag");
	GLuint programB = loadProgram("shaders/poop.vert", "shaders/poop.frag");

	GLint u_Transform = glGetUniformLocation(programA, "u_Transform");
	GLint u_Projection = glGetUniformLocation(programA, "u_Projection");
	GLint u_View = glGetUniformLocation(programA, "u_View");
	GLint u_Time = glGetUniformLocation(programA, "u_Time");

	float time = 0.f;
	
	while (!glfwWindowShouldClose(window))
	{
		float angle = deg2rad * time;

		// Calculate projection
		float aspect = (float) windowWidth / (float) windowHeight;
		float scaleFactor = 1.f / 5.f;
		Matrix projection =
		{
			scaleFactor, 0.f, 0.f,
			0.f, scaleFactor * aspect, 0.f,
			0.f, 0.f, 1.f
		};

		glUniformMatrix3fv(u_Projection, 1, false, (GLfloat*)&projection);

		float cameraX = sin(time) * 2.f;
		float cameraY = 0.5f;

		cameraX = 0.f;
		cameraY = 0.f;
		Matrix view =
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			-cameraX, -cameraY, 1.f
		};

		glUniformMatrix3fv(u_View, 1, false, (GLfloat*)&view);

		Matrix rotation =
		{
			cos(time), -sin(time), 0.f,
			sin(time), cos(time), 0.f,
			0.f, 0.f, 1.f
		};

		Matrix translate =
		{
			2.f, 0.f, 0.f,
			0.f, 2.f, 0.f,
			0.f, 0.f, 1.f
		};

		Matrix scale =
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f
		};

		Matrix transform = translate * scale;

		time += 0.01f;

		glfwPollEvents();

		glClearColor(0.5f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programA);
		glUniform1f(glGetUniformLocation(programA, "u_Time"), time);
		glUniformMatrix3fv(glGetUniformLocation(programA, "u_Transform"), 1, false, (float*)&transform);
		glBindVertexArray(triangleA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*glUseProgram(programB);
		glUniform1f(glGetUniformLocation(programB, "u_Time"), time);
		glUniform2f(glGetUniformLocation(programB, "u_Offset"), -sin(time) * 0.5f, -cos(time) * 0.5f);
		glBindVertexArray(triangleB);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);*/

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}