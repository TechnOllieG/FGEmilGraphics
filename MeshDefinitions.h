#pragma once

const float quad_Data[] =
{
	-0.5f, -0.5f,	1.f, 0.f, 0.f,
	0.5f, -0.5f,	0.f, 1.f, 0.f,
	0.5f, 0.5f,		0.f, 0.f, 1.f,
	-0.5f, 0.5f,	1.f, 1.f, 0.f
};

const unsigned int quad_Index_Data[] =
{
	0, 1, 2,
	0, 2, 3
};

const float triangle_Data[] =
{
	0.2f, 0.5f,		0.f, 0.5f, 0.5f,
	-0.3f, 0.4f,	0.5f, 0.5f, 0.5f,
	0.f, -0.1f,		0.f, 0.f, 0.8f
};

const unsigned int triangle_Index_Data[] =
{
	0, 1, 2
};