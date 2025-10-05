//Version number
#version 400
//Layout Qualifer
out vec4 fragcolor;
//Unfrom variable
uniform float randColourX;
uniform float randColourY;
uniform float randColourZ;

uniform sampler2D diffuse;

in GS_OUT{
	vec2 texCoords;
}gs_in;
 
void main()
{
//Setting each vector component to uniform varaible then setting final colour
	vec4 color;
	color = vec4(randColourX,randColourY,randColourZ,1.0);

    fragcolor = texture(diffuse, gs_in.texCoords) + color;
}