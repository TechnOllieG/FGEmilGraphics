#pragma once
#include "MeshDefinitions.h"

enum class Mesh
{
	Quad,
	Cube,

	MAX // Always leave this last
};

extern const float* mesh_Data[];

const unsigned int mesh_Data_Size[]
{
	sizeof(quad_Data),
	sizeof(cube_Data),
};

extern const unsigned int* mesh_Indices[];

const unsigned int mesh_Indices_Size[]
{
	sizeof(quad_Index_Data),
	sizeof(cube_Index_Data),
};