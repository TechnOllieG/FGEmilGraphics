#version 330 core

in vec3 f_Color;
in vec2 f_TexCoord;

out vec4 o_Color;

void main()
{
	o_Color = vec4(f_Color, 1.0f);
	o_Color = vec4(f_TexCoord, 0.f, 1.f);
}