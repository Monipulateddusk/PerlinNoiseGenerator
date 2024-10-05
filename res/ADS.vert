#version 400 core

// Inputs 
layout (location = 0) in vec3 VertexPosition; // Vertex Pos
layout (location = 1) in vec3 aNormal; // Vertex normals

// Uniforms (imported values from CPU)
uniform mat4 model; // Model matrix
uniform mat4 transform; // Transformation Matrix (MVP)

// Outputs to Frag shader (NAMES DON'T MATTER, ONLY DATA TYPE)
out vec3 FragPos; // Fragment pos in world space
out vec3 Normal; // Transformed normal (used for lighting)

void main()
{
   	FragPos = vec3(model * vec4(VertexPosition, 1));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = transform * vec4(VertexPosition, 1);
}