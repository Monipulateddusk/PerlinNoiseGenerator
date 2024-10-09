#version 400 core

// Inputs from vertex shader
in vec3 FragPos;
in vec2 TextureCoord;
in vec3 Normal;

// Uniforms (inputs from CPU)
uniform vec3 lightPos;
uniform vec3 lightColour; 
uniform vec3 objectColour;

uniform vec3 viewPosition;
uniform sampler2D diffuse;

// Output colour
out vec4 FragColour;

void main(){
	
	// Calculation of Ambient lighting
	vec3 ambientLight = .1f * lightColour;

	// Calculation of Diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos); // Calculating the light direction vector by subtracting the position between the origin and the target
	float diffuseLightStrength = max(dot(norm, lightDirection), 0.0);

	vec3 diffuseLight = diffuseLightStrength * lightColour;

	// Calculation of Specular lighting
	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 halfwayDir = normalize(lightDirection + viewDir);

	float specularLightStrength = pow(max(dot(norm, halfwayDir), 0), 1f);
	vec3 specularLight = lightColour * specularLightStrength;

	// Setting the output doing specular lighting
	vec3 resultLight = (ambientLight + diffuseLight + specularLight) * objectColour;

	// Calculating the texture
	vec4 tex = texture(diffuse, TextureCoord);

	// Applying the texure to the lighting system
	FragColour = vec4(resultLight, 1.0) + tex;


}