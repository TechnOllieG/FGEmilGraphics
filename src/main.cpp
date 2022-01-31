#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Library.h"
#include "Camera.h"
#include "Mesh.h"
#include "Engine.h"

using namespace glm;

int main()
{
	engInit();

	GLuint testShader = getProgram(ShaderProgram::Test);

	glUseProgram(testShader);
	GLint u_Model = glGetUniformLocation(testShader, "u_Model");
	GLint u_Projection = glGetUniformLocation(testShader, "u_Projection");
	GLint u_View = glGetUniformLocation(testShader, "u_View");
	GLint u_Time = glGetUniformLocation(testShader, "u_Time");

	Camera camera;
	camera.position = vec3(-10.f, 1.f, 5.f);

	float time = 0.f;
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time += 0.01f;
		glUniform1f(u_Time, time);

		// Update view + projection
		mat4 identity(1.f);

		float aspect = (float)windowWidth / (float)windowHeight;
		mat4 projection = perspective(radians(90.f), aspect, 0.1f, 100.f);
		//projection = ortho(-5.f * aspect, 5.f * aspect, -5.f, 5.f, -5.f, 10.f);

		glUniformMatrix4fv(u_Projection, 1, false, (GLfloat*)&projection);

		// View matrix
		mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(u_View, 1, false, (GLfloat*)&view);

		// Render grass
		{
			mat4 model;// = translate(mat4(1.f), vec3(0.f, 0.f, -1.f));
			model = rotate(mat4(1.f), -pi<float>() / 2.f, vec3(1.f, 0.f, 0.f));
			model = scale(model, vec3(50.f));

			glBindVertexArray(getMesh(Mesh::Quad));
			glBindTexture(GL_TEXTURE_2D, getTexture(Texture::Grass));
			glUniformMatrix4fv(u_Model, 1, false, (GLfloat*)&model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Render cube
		{
			mat4 model;// = translate(mat4(1.f), vec3(0.f, 0.f, -1.f));
			model = translate(mat4(1.f), vec3(0.f, 1.5f, 0.f)) *rotate(mat4(1.f), time, vec3(2.f, 1.f, -3.2f));

			glBindVertexArray(getMesh(Mesh::Cube));
			glBindTexture(GL_TEXTURE_2D, getTexture(Texture::Gravel));
			glUniformMatrix4fv(u_Model, 1, false, (GLfloat*)&model);
			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
		}

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}