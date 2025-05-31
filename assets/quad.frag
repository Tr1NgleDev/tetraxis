#version 450 core

out vec4 color;

in vec2 uv;

uniform sampler2D tex;

void main()
{
	color = texture(tex, uv);
	color.a = 1.0;
}
