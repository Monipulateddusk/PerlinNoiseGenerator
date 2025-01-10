#version 400

in VS_OUT {
	vec3 v_norm;
}vs_in;

out vec4 fragColour;
 
 
void main() {
 
 	float intensity = pow(0.5 - dot(vs_in.v_norm, vec3(1.0,1.0,1.0)),1.0);

    fragColour = vec4(0.7,0.1,0.1,intensity);
}