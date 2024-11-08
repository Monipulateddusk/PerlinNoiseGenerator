#version 420 core

in Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vs_In;

layout (binding = 0) uniform samplerCube Skybox;
layout (binding = 1) uniform sampler2D diffuse;
uniform vec3 cameraPos;

out vec4 FragColour;

void main()
{
    vec3 dir = cameraPos - vs_In.Position;

    vec3 R = reflect(dir, normalize(vs_In.Normal)); // Calculate the reflection dir for the incident vector

    vec4 colour = vec4(texture(Skybox, R).rgb,1.0); // Using the reflection vector to read the rgb value from the Skybox

    vec4 tex = texture(diffuse, vs_In.tC);

    FragColour = colour + tex;

}
