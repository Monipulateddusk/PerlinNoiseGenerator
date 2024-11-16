#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Skybox.h"
#include "FrameBufferObject.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void initQuadVAO();

	void processInput();
	void update();
	void gameLoop();

	void linkADS();
	void linkGeoShader();
	void linkEnviroMapping();

	void renderFBO();
	void renderMonkey();
	void renderSkybox();
	void renderActiveShader();
	void drawGame();



	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Camera myCamera;
	Texture texture; 
	Shader shader;
	Shader ADS; 
	Shader geomShader;
	Shader enviroMappingShader;
	Shader FBOShader;
	Shader grayScaleShader; 
	Shader edgeDetectionShader;
	Skybox skybox;
	FrameBufferObject* FBO;
	GLuint quadVAO;
	GLuint quadVBO;

	float counter;
	bool isADSEnabled;


};

