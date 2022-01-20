#pragma once
#include "Library.h"

enum class ShaderName
{
	test,
	poop,
	MAX // Always leave last
};

ShaderProgramSource shaderSources[] =
{
	ShaderProgramSource("shaders/test.vert", "shaders/test.frag"),
	ShaderProgramSource("shaders/poop.vert", "shaders/poop.frag")
};