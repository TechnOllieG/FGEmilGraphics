#pragma once
#include <GL/glew.h>

struct Vector2
{
	float x;
	float y;

	Vector2()
	{
		x = 0.f;
		y = 0.f;
	}
};

class Object
{
public:
	bool isValid = false;
	int vaoIndex = -1;
	Vector2 position = Vector2();
	int shaderProgramIndex = -1;
};

class Mesh
{
public:
	void* data = nullptr;
	unsigned int dataSize = -1;
	void* elementData = nullptr;
	unsigned int elementDataSize = -1;
	bool isValid = false;

	Mesh()
	{
		isValid = false;
	}

	Mesh(void* in_data, unsigned int in_dataSize, void* in_elementData, unsigned int in_elementDataSize)
	{
		data = in_data;
		dataSize = in_dataSize;
		elementData = in_elementData;
		elementDataSize = in_elementDataSize;
		isValid = true;
	}

	bool operator ==(Mesh rhs)
	{
		return data == rhs.data && dataSize == rhs.dataSize && elementData == rhs.elementData && elementDataSize && rhs.elementDataSize;
	}
};

struct ShaderProgramSource
{
	const char* vertexPath = nullptr;
	const char* fragmentPath = nullptr;
	bool isValid = false;

	ShaderProgramSource()
	{
		isValid = false;
	}

	ShaderProgramSource(const char* in_vertexPath, const char* in_fragmentPath)
	{
		vertexPath = in_vertexPath;
		fragmentPath = in_fragmentPath;
		isValid = true;
	}

	bool operator ==(ShaderProgramSource rhs)
	{
		return vertexPath == rhs.vertexPath && fragmentPath == rhs.fragmentPath;
	}
};