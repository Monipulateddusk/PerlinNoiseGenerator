#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 texCoords;
}vs_in;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = vec4(texture(screenTexture, vs_in.texCoords));
}