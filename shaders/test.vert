#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform float u_Time;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 f_Color;
out vec2 f_TexCoord;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 0.f, 1.f);
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
}