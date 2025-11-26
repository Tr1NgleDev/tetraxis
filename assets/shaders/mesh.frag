#version 450 core

in vec4 fsNormal;
in vec4 fsColor;
in vec3 fsUVW;

out vec4 color;

void main()
{
	vec4 col = fsColor;

	col.rgb *= max(dot(fsNormal, vec4(0.0, 1.0, 0.0, 0.0)), 0.1);

	color = col;
}
