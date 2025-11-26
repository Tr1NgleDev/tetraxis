#version 450 core

out vec4 color;

in vec2 uv;

uniform sampler2D tex;
uniform sampler2D depth;

void main()
{
	color = texture(tex, uv);
}
