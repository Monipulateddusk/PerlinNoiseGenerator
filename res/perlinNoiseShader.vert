#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextC;

out VS_OUT {
	vec2 texCoords;
	vec3 vert_norm;
	vec4 vert_pos;
}vs_out;

uniform mat4 transform;

void main()
{
	vs_out.vert_pos = transform * vec4(VertexPosition, 1.0);
	vs_out.texCoords = TextC;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}