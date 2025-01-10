#version 400 core

uniform sampler2D grass, snow, rock, sand, water; // Water has index of 1, sand = 2, grass = 3, rock = 4, snow = 5. Water is the default

in VS_OUT {
	vec2 texCoords;
	float height;
	float norm_height;
	float next_Norm_Height;
}vs_in;

out vec4 FragColor; // 'out' is used for fragment output

struct SAMPLERS{
	int priorSampIndex; // Most important sampler2D
	int irrSampIndex; // Less important sampler2D
	float intensity;	// Ranges from 0-1 and determines how strong the priority sampler is over the irrelivent one
						//I.e. if the intensity = 0, the irrSampIndex is more strong
						// intensity = 1, priorSampIndex is strong
						// intensity = 0.f, a 50/50 mix of the two is displayed																
};

float normaliseInRange(float minim, float maxim, float v){
	return clamp((v-minim) / (maxim - minim), 0.0, 1.0);
};

sampler2D IndexToSampler(int index){
	switch(index){
		case 0:
			return water;
		break;

		case 1:
			return water;
		break;
		case 2:
			return sand;
		break;
		case 3:
			return grass;
		break;
		case 4:
			return rock;
		break;
		case 5:
			return snow;
		break;
		default:
			return water;
		break;
	};
};

// Determine the colouration based on the normal height - which ranges from 0-1.
// Basically  if the value is close to 0 it would be blue, mid way between 0.2 and 0.4 would be yellow, etc
vec3 HeightToColour(float height){
	if(vs_in.norm_height < 0.05){
		return vec3(0,0,1);
	};
	if(vs_in.norm_height > 0.05 && vs_in.norm_height < 0.1 ){
		return vec3(1,1,0);
	};
	if(vs_in.norm_height > 0.1 && vs_in.norm_height < 0.25){
		return vec3(0,1,0);
	};
	if(vs_in.norm_height > 0.25 && vs_in.norm_height < 0.8 ){
		return vec3(.2,.2,.2);
	};
	if(vs_in.norm_height > 0.8){
		return vec3(1,1,1);
	};
};

SAMPLERS HeightToSampler(float height){
	SAMPLERS samplers;
	// Water
	if(vs_in.norm_height < 0.05){
		samplers.priorSampIndex = 1;
		samplers.irrSampIndex = 2;
		samplers.intensity = normaliseInRange(0, 0.05, height);
		return samplers;
	};
	// Sand
	if(vs_in.norm_height > 0.05 && vs_in.norm_height < 0.1 ){
		samplers.priorSampIndex = 2;
		samplers.irrSampIndex = 3;
		samplers.intensity = normaliseInRange(0.05,  0.1, height);
		return samplers;
	};
	// Grass
	if(vs_in.norm_height > 0.1 && vs_in.norm_height < 0.25){
		samplers.priorSampIndex = 3;
		samplers.irrSampIndex = 4;
		samplers.intensity = normaliseInRange(0.1 , 0.25, height);
		return samplers;
	};
	// Rock
	if(vs_in.norm_height > 0.25 && vs_in.norm_height < 0.6 ){
		samplers.priorSampIndex = 4;
		samplers.irrSampIndex = 5;
		samplers.intensity = normaliseInRange(0.25, 0.6, height);
		return samplers;
	};
	// Ice
	if(vs_in.norm_height > 0.6){
		samplers.priorSampIndex = 5;
		samplers.irrSampIndex = 5;
		samplers.intensity = normaliseInRange(0.6, 1, height);
		return samplers;
	};
};

void main()
{
	SAMPLERS samplers;
	samplers = HeightToSampler(vs_in.norm_height);
	vec4 samp1 = texture(IndexToSampler(samplers.priorSampIndex), vs_in.texCoords);
	vec4 samp2 = texture(IndexToSampler(samplers.irrSampIndex), vs_in.texCoords);

	FragColor = mix(samp1, samp2, samplers.intensity);

//	vec4 tex = texture(HeightToSampler(vs_in.norm_height), vs_in.texCoords);
//
//    FragColor = tex;
}
