#version 330 core

uniform sampler2D u_SamplerA;
uniform sampler2D u_SamplerB;
uniform float u_Time;

in vec3 f_Color;
in vec2 f_TexCoord;

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

	vec4 a = texture(u_SamplerA, f_TexCoord);
	vec4 b = texture(u_SamplerB, f_TexCoord);

	float blendFactor = sin((u_Time + f_TexCoord.x * 10) * 2) * 0.5 + 0.5;

	vec4 final = mix(a, b, blendFactor);

	o_Color = final;
}