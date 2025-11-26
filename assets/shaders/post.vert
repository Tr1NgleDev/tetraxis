#version 450 core

layout(location = 0) in vec2 vert;

out vec2 uv;

void main()
{
	uv = vert;
	gl_Position = vec4(vert * vec2(2.0) - vec2(1.0), 0.0, 1.0);
}
