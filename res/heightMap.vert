#version 330 core
/*  Help for the shader found here: https://padrend.github.io/Tutorials/heightmap_shader */
layout (location = 0) in vec3 VertexPosition; // Vertex Pos
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 VertexNormal; // Vertex normals

//Uniform variable
uniform mat4 transform;
uniform sampler2D heightMap;
float scale =  1.0;
float offset = 0.0;



out vec2 TexCoords;
varying float height; // Varying means that this will be passed down to the fragment shader but needs to be computed in the vertex. 
varying float normalisedHeight;

void main()
{
    normalisedHeight = texture(heightMap, aTexCoords).y;
    height = offset + scale * normalisedHeight;
    
    vec4 pos =  vec4(VertexPosition, 1.0);
    pos.y = pos.y + height;

    gl_Position = transform * pos;
    TexCoords = aTexCoords;
}  