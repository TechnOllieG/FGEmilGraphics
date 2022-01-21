#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

uniform float u_Time;
uniform mat3 u_Transform;

out vec3 f_Color;

void main()
{
	gl_Position = vec4(u_Transform * vec3(a_Position, 1.f), 1.0f);
	f_Color = a_Color;
}