#version 400 core

in VS_OUT {
	vec3 texCoords;
}vs_in;

uniform samplerCube skybox;

out vec4 FragColour;

void main()
{
	FragColour = texture(skybox, vs_in.texCoords);
}
