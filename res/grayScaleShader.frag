#version 330 core
out vec4 FragColor;
  
in VS_OUT {
	vec2 texCoords;
}vs_in;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = texture(screenTexture, vs_in.texCoords);
    float average = (0.2126 * FragColor.r) + (0.7152 * FragColor.g) + (0.0722 * FragColor.b);
    FragColor = vec4(average, average, average, 1.0);
}