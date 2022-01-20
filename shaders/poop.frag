#version 330 core

in vec3 f_Color;
out vec4 o_Color;

uniform float u_Time;

void main()
{
	float pulse = cos(u_Time * 10.0f) * 0.4f + 0.5f;
	o_Color = vec4(0.f, 1.f, 0.f, 1.0f);
}