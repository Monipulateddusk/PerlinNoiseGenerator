#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
	vec2 texCoords;
}vs_out;

void main()
{
    gl_Position = vec4(VertexPosition.x, VertexPosition.y, 0.0, 1.0); 
    vs_out.texCoords = VertexTexCoord;
}  