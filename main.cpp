#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const char* VERTEX_SRC = ""
"#version 330 core\n"
"in vec2 a_Position;"
"uniform vec2 u_Offset;"
"void main()"
"{"
"	gl_Position = vec4(a_Position + u_Offset, 0.0f, 1.0f);"
"}";

const char* FRAGMENT_SRC = ""
"#version 330 core\n"
"out vec4 o_Color;"
"void main()"
"{"
"	o_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
"}";

GLuint loadShader(GLenum type, const char* src)
{
	static char infoBuffer[1024];

	// Shaders!
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
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

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics Course", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewInit();

	// Setting up the vertex buffer! :)
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	float vertexData[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.f, 0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SRC);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SRC);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	GLint u_Offset = glGetUniformLocation(program, "u_Offset");

	float time = 0.f;

	while (!glfwWindowShouldClose(window))
	{
		time += 0.01f;
		glfwPollEvents();

		glClearColor(0.5f, 0.2f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform2f(u_Offset, sin(time), cos(time));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}