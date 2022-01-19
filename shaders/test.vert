#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;
uniform vec2 u_Offset;
uniform float u_Time;

out vec3 f_Color;

void main()
{
	vec2 position = a_Position;
	position.x = a_Position.x * cos(u_Time) - a_Position.y * sin(u_Time);
	position.y = a_Position.x * sin(u_Time) + a_Position.y * cos(u_Time);

	gl_Position = vec4(position + u_Offset, 0.0f, 1.0f);
	f_Color = a_Color;
}