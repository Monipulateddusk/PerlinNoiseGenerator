#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec2 texCoord0;  // 'varying' is replaced with 'out'

out VS_OUT {
	vec2 texCoords;
}vs_out;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(VertexPosition, 1.0);
	vs_out.texCoords = VertexTexCoord;  // Pass the texture coordinates to the fragment shader
}
