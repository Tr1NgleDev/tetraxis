#version 450 core

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 gsNormalMV[];
in vec4 gsColor[];
in vec3 gsUVW[];

out vec4 fsNormal;
out vec4 fsColor;
out vec3 fsUVW;

uniform mat4 P;

// tetrahedron edge vertex indices
const ivec2 edges[6] = ivec2[6](ivec2(0, 1), ivec2(0, 2), ivec2(0, 3), ivec2(1, 2), ivec2(1, 3), ivec2(2, 3));

void main()
{
	// calculate intersection between simplex and hyperplane (3D view)

	// number of intersection points
	int k = 0;

	for (int i = 0; i < 6; ++i)
	{
		if (k == 4)
		{
			break;
		}

		ivec2 e = edges[i];
		vec4 p0 = gl_in[e[0]].gl_Position;
		vec4 p1 = gl_in[e[1]].gl_Position;

		// vertices are on the same side of the hyperplane so the edge doesn't intersect
		if ((p0.w < 0.0000001 && p1.w < 0.0000001) || (p0.w > -0.0000001 && p1.w > -0.0000001))
		{
			continue;
		}

		// intersection
		float a = 0;
		if (abs(p1.w - p0.w) > 0.0000001)
		{
			a = (-p0.w) / (p1.w - p0.w);
		}

		vec4 pt = mix(p0, p1, a);
		gl_Position = P * vec4(pt.xyz, 1.0);

		fsNormal = mix(gsNormalMV[e[0]], gsNormalMV[e[1]], a);
		fsColor = mix(gsColor[e[0]], gsColor[e[1]], a);
		fsUVW = mix(gsUVW[e[0]], gsUVW[e[1]], a);

		EmitVertex();

		++k;
	}
	
	EndPrimitive();
}
