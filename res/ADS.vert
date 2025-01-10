#version 400 core

// Inputs 
layout (location = 0) in vec3 VertexPosition; // Vertex Pos
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 VertexNormal; // Vertex normals

// Uniforms (imported values from CPU)
uniform mat4 transform; // Transformation Matrix (MVP). Automatically attached via the Shader.Update script taking in the transform's getModel to return the MVP
uniform mat4 model; // Model matrix

out VS_OUT {
	vec2 texCoords;
    vec3 fragPos;
    vec3 normal;
}vs_out;

void main()
{
   	vs_out.fragPos = vec3(model * vec4(VertexPosition, 1));
    vs_out.normal = mat3(transpose(inverse(model))) * VertexNormal;

    gl_Position = transform * vec4(VertexPosition, 1);
    vs_out.texCoords = TexCoord;

}