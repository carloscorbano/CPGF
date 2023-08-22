#version 450 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out vec3 oColor;
out vec2 oTexCoord;

layout (std140) uniform CameraBlock
{
	uniform mat4 view;
	uniform mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(iPos, 1.0f);
	oColor = iColor;
	oTexCoord = iTexCoord;
}