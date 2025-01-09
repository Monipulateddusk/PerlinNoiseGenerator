#version 400 core

in vec2 TexCoords;  
//uniform sampler2D colourDiffuse;

varying float height; // Varying means that this will be passed down to the fragment shader but needs to be computed in the vertex. 
varying float normalisedHeight;

out vec4 FragColor; // 'out' is used for fragment output

void main()
{
    FragColor = vec4(normalisedHeight, normalisedHeight, normalisedHeight, 1);
    //FragColor = texture(colourDiffuse, texCoord0);  // Use 'texture' instead of 'texture2D'
}
