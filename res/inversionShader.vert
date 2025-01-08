#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform sampler2D heightMap;

out vec2 TexCoords;


void main()
{
    vec4 heightMapSample = texture2D(heightMap, aTexCoords);
    float height = heightMapSample.y;

    float newPositionY =  aPos.y + height;
    
    gl_Position = vec4(aPos.x, newPositionY, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  