#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdlib.h>

const float triangleA_Data[] =
{
	-0.5f, -0.5f,	1.f, 0.f, 0.f,
	0.5f, -0.5f,	0.f, 1.f, 0.f,
	0.5f, 0.5f,		0.f, 0.f, 1.f,
	-0.5f, 0.5f,	1.f, 1.f, 0.f
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

void handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));

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

	GLuint programA = loadProgram("shaders/test.vert", "shaders/test.frag");
	GLuint programB = loadProgram("shaders/poop.vert", "shaders/poop.frag");

	float time = 0.f;

	while (!glfwWindowShouldClose(window))
	{
		time += 0.01f;

		glfwPollEvents();

		glClearColor(0.5f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programA);
		glUniform1f(glGetUniformLocation(programA, "u_Time"), time);
		glUniform2f(glGetUniformLocation(programA, "u_Offset"), sin(time) * 0.5f, cos(time) * 0.5f);
		glBindVertexArray(triangleA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(programB);
		glUniform1f(glGetUniformLocation(programB, "u_Time"), time);
		glUniform2f(glGetUniformLocation(programB, "u_Offset"), -sin(time) * 0.5f, -cos(time) * 0.5f);
		glBindVertexArray(triangleB);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}