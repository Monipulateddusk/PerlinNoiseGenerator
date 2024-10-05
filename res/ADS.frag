#version 330 core

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;

// Uniforms (inputs from CPU)
uniform vec3 lightPos;
uniform vec3 lightColour; 
uniform vec3 objectColour;

// Output colour
out vec4 FragColour;

void main(){
	
	// Calculation of Ambient lighting
	vec3 ambientLight = .5f * lightColour;

	// Calculation of Diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos); // Calculating the light direction vector by subtracting the position between the origin and the target
	float diffuseLightStrength = max(dot(norm, lightDirection), 0.0);

	vec3 diffuseLight = diffuseLightStrength * lightColour;

	// Setting the output without doing specular lighting
	vec3 resultLight = (ambientLight + diffuseLight) * objectColour;
	FragColour = vec4(resultLight, 1.0);
}