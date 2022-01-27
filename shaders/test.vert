#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform float u_Time;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec2 f_TexCoord;
out vec3 f_Normal;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.f);
	f_TexCoord = a_TexCoord;
	f_Normal = (u_Model * vec4(a_Normal, 0.f)).xyz;
}