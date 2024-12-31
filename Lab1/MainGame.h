#pragma once
#include "DEFINITIONS.h"
#include "Display.h" 
#include "Shader.h"
#include "Skybox.h"
#include "FrameBufferObject.h"
#include "GameObject.h"
#include "PerlinNoiseGenerator.h"
#include "UIButton.h"

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
	void linkNoiseShader();

	void renderFBO();
	void renderMonkey();
	void renderSkybox();
	void renderActiveShader();
	
	void drawUIElements();
	void drawGame();



	Display _gameDisplay;
	GameState _gameState;

	#pragma region Shaders

	Shader shader;
	Shader ADS;
	Shader geomShader;
	Shader enviroMappingShader;
	Shader FBOShader;
	Shader grayScaleShader;
	Shader edgeDetectionShader;
	Shader noiseShader;
	Shader glowShader;

	#pragma endregion

#pragma region UI
	BaseUserInterfaceElement* elementSelected;
	MouseState mouseState;

	UIButton* tempButton;
#pragma endregion


	Texture noiseTexture, lavaTexture;

	Skybox skybox;
	GameObject* monkey;

	PerlinNoiseGenerator noiseGen;


	
	//Mesh mesh1;
	//Mesh mesh2;
	Camera myCamera;
	//Texture texture; 

	FrameBufferObject* FBO;
	//Transform transform;
	GLuint quadVAO;
	GLuint quadVBO;

	float counter;
	bool isADSEnabled;

	unsigned int perlinNoiseSeedValue;
};

