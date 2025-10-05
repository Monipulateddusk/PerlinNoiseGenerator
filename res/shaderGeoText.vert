//Version Number
#version 400 core

//The layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

//Uniform variable
uniform mat4 transform;

out VS_OUT {
	vec2 texCoords;
	vec3 v_norm;
	vec4 v_pos;
}vs_out;

void main()
{
	//Assigning the normal and position data
	vs_out.v_norm = VertexNormal;
	vs_out.v_pos = vec4(VertexPosition, 1.0);
	vs_out.texCoords = VertexTexCoord;

	// Sets the position of the current vertex
	gl_Position = transform * vec4(VertexPosition, 1.0);
}