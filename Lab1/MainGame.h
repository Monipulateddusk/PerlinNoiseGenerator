#pragma once
#include "DEFINITIONS.h"
#include "Display.h" 
#include "Shader.h"
#include "Skybox.h"
#include "FrameBufferObject.h"
#include "GameObject.h"
#include "PerlinNoiseGenerator.h"
#include "UIButton.h"
#include "UISlider.h"

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
	void initUI();

	void processInput();
	void update();
	void gameLoop();

	void activateOrtho();
	void disableOrtho();

	void linkADS();
	void linkGeoShader();
	void linkEnviroMapping();
	void linkNoiseShader();
	void linkHeightMapShader();

	void renderEnvironmentMonkey();
	void renderExplosionSphere();
	void renderFBO();
	void renderMonkey();
	void renderSkybox();
	void renderActiveShader();

	void setPerlinNoiseTexture();
	
	void drawBackgroundUI();
	void drawUIElements();
	void drawGeneratedPerlinNoise();
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
	Shader heightMapShader;

	#pragma endregion

#pragma region UI
	std::shared_ptr<BaseUserInterfaceElement> elementSelected;
	MouseState mouseState;

	std::vector<std::shared_ptr<BaseUserInterfaceElement>> uiElements;
	Texture generatedPerlinNoiseTexture;
#pragma endregion


	Texture noiseTexture, lavaTexture;

	Skybox skybox;
	GameObject* monkey, *cube, *plane, *sphere;

	PerlinNoiseGenerator noiseGen;

	Camera myCamera;

	FrameBufferObject* FBO;
	GLuint quadVAO;
	GLuint quadVBO;

	float counter;
	bool isADSEnabled;

	unsigned int perlinNoiseSeedValue;
};

