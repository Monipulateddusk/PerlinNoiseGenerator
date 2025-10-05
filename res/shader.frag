#version 400 core

uniform sampler2D diffuse;

in VS_OUT {
	vec2 texCoords;
}vs_in;

out vec4 FragColor; // 'out' is used for fragment output

void main()
{
    FragColor = texture(diffuse, vs_in.texCoords);  // Use 'texture' instead of 'texture2D'
}
