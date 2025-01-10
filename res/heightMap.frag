#version 400 core

//uniform sampler2D colourDiffuse;

in VS_OUT {
	vec2 texCoords;
	float height;
	float norm_height;
}vs_in;

out vec4 FragColor; // 'out' is used for fragment output

void main()
{
    FragColor = vec4(vs_in.norm_height, vs_in.norm_height, vs_in.norm_height, 1);
    //FragColor = texture(colourDiffuse, texCoord0);  // Use 'texture' instead of 'texture2D'
}
