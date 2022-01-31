#version 330 core
uniform sampler2D u_Sampler;
uniform float u_Time;

// Lighting
uniform vec3 u_DirLight;
uniform vec3 u_EyePosition;

in vec2 f_TexCoord;
in vec3 f_Normal;
in vec3 f_WorldPosition;

out vec4 o_Color;

const vec3 AMBIENT_COLOR = vec3(0.05f, 0.15f, 0.10f);

void main()
{
	vec4 color = texture(u_Sampler, f_TexCoord);
	float diffuse = clamp(-dot(f_Normal, u_DirLight), 0.0, 1.0);

	vec3 eyeDirection = normalize(f_WorldPosition - u_EyePosition);
	vec3 middle = -normalize(u_DirLight + eyeDirection);
	float specular = dot(middle, f_Normal);
	specular = clamp(specular, 0, 1);
	specular = pow(specular, 128);
	
	o_Color = vec4(color.rgb * (diffuse + specular) + color.rgb * AMBIENT_COLOR, color.a);
}