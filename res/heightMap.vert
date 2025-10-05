#version 400 core
/*  Help for the shader found here: https://padrend.github.io/Tutorials/heightmap_shader */
layout (location = 0) in vec3 VertexPosition; // Vertex Pos
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 VertexNormal; // Vertex normals

//Uniform variable
uniform mat4 transform;
uniform sampler2D heightMap;
float scale =  1.0;
float offset = 1.0;

out VS_OUT {
	vec2 texCoords;
	float height;
	float norm_height;
    float next_Norm_Height;
}vs_out;

void main()
{
    vs_out.norm_height = texture(heightMap, aTexCoords).y;
    vs_out.next_Norm_Height = texture(heightMap, vec2(aTexCoords.x + offset, aTexCoords.y + offset)).y;

    vs_out.height = scale * vs_out.norm_height;
    
    vec4 pos =  vec4(VertexPosition, 1.0);
    pos.y = pos.y + vs_out.height;

    gl_Position = transform * pos;
    vs_out.texCoords = aTexCoords;
}  