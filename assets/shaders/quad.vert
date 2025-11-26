#version 450 core

layout(location = 0) in vec2 vert;

out vec2 uv;

uniform mat4 model;
uniform mat4 proj;

void main()
{
	uv = vert;
	gl_Position = proj * model * vec4(vert, 0.0, 1.0);
}
