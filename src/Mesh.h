#pragma once
#include "MeshDefinitions.h"

enum class Mesh
{
	Quad,
	Cube,

	MAX // Always leave this last
};

const float* mesh_Data[]
{
	quad_Data,
	cube_Data,
};

const unsigned int mesh_Data_Size[]
{
	sizeof(quad_Data),
	sizeof(cube_Data),
};

const unsigned int* mesh_Indices[]
{
	quad_Index_Data,
	cube_Index_Data,
};

const unsigned int mesh_Indices_Size[]
{
	sizeof(quad_Index_Data),
	sizeof(cube_Index_Data),
};