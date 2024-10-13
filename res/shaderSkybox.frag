#version 400 core

in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 FragColour;

void main()
{
	FragColour = texture(skybox, TexCoords);
}
