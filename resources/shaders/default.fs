#version 450 core
out vec4 FragColor;

struct Material
{
	sampler2D tex_diffuse;
	sampler2D tex_specular;
	sampler2D tex_normal;
	sampler2D tex_ambient;
};

in vec3 oColor;
in vec2 oTexCoord;

uniform Material material;

void main()
{
	FragColor = vec4(oColor * texture(material.tex_diffuse, oTexCoord).rgb, 1.0);
}