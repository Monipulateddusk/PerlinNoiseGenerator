#version 400
//The layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

//Uniform variable
uniform mat4 transform;

out VS_OUT {
	vec3 v_norm;
}vs_out;


void main()
{
	vs_out.v_norm = VertexNormal; //static

	//vs_out.v_norm = mat3(transpose(inverse(view*model))) * VertexNormal;

	gl_Position = transform * vec4(VertexPosition, 1.0);
}