#version 450 core

layout(location = 0) in vec4 vert;
layout(location = 1) in vec4 norm;
layout(location = 2) in vec4 col;
layout(location = 3) in vec3 uvw;

out vec4 gsNormalMV;
out vec4 gsColor;
out vec3 gsUVW;

uniform float[25] MV;

vec4 Mat5_multiply(in float m[25], in vec4 v, in float finalComp)
{
	return vec4(
		m[0*5+0] * v[0] + m[1*5+0] * v[1] + m[2*5+0] * v[2] + m[3*5+0] * v[3] + m[4*5+0] * finalComp,
		m[0*5+1] * v[0] + m[1*5+1] * v[1] + m[2*5+1] * v[2] + m[3*5+1] * v[3] + m[4*5+1] * finalComp,
		m[0*5+2] * v[0] + m[1*5+2] * v[1] + m[2*5+2] * v[2] + m[3*5+2] * v[3] + m[4*5+2] * finalComp,
		m[0*5+3] * v[0] + m[1*5+3] * v[1] + m[2*5+3] * v[2] + m[3*5+3] * v[3] + m[4*5+3] * finalComp
	);
}

void main()
{
	// multiply the vertex by MV
	vec4 result = Mat5_multiply(MV, vert, 1.0);

	mat4 MVM4 = transpose(inverse(mat4(
		vec4(MV[0 * 5 + 0], MV[0 * 5 + 1], MV[0 * 5 + 2], MV[0 * 5 + 3]),
		vec4(MV[1 * 5 + 0], MV[1 * 5 + 1], MV[1 * 5 + 2], MV[1 * 5 + 3]),
		vec4(MV[2 * 5 + 0], MV[2 * 5 + 1], MV[2 * 5 + 2], MV[2 * 5 + 3]),
		vec4(MV[3 * 5 + 0], MV[3 * 5 + 1], MV[3 * 5 + 2], MV[3 * 5 + 3])
	)));

	gl_Position = result;
	gsNormalMV = normalize(MVM4 * norm);
	gsColor = col;
	gsUVW = uvw;
}
