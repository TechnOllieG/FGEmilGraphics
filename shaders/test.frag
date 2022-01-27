#version 330 core

uniform sampler2D u_Sampler;
uniform float u_Time;

in vec2 f_TexCoord;
in vec3 f_Normal;

out vec4 o_Color;

void main()
{
//	o_Color = vec4(f_Color, 1.0f);
//	o_Color = vec4(f_TexCoord, 0.f, 1.f);
//
//	vec2 texCoord = f_TexCoord;
//
//	texCoord *= sin(u_Time) * 0.5f + 0.5f;
//
//	texCoord.x += u_Time;
//	texCoord.y += cos(u_Time);
//	o_Color = texture(u_SamplerA, texCoord);

	o_Color = texture(u_Sampler, f_TexCoord);
	o_Color = vec4(f_Normal, 1.f);
}