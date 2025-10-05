#pragma once
#include "DEFINITIONS.h"
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

using namespace std;
class Skybox
{
public: 
	Skybox() : textureID(0), skyboxVAO(0), skyboxVBO(0) {}
	~Skybox(){}
	void init(vector<string> faces);
	void draw(Camera* cam);

	unsigned int textureID;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

private:
	Shader shader;
};

