#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>



Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
					Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
					Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)) };

unsigned int indices[] = { 0, 1, 2 };

Transform transform;

MainGame::MainGame()
{
	counter = 0; isADSEnabled = false;
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
}

MainGame::~MainGame()
{

}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//mesh1.init(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])); //size calcuated by number of bytes of an array / no bytes of one element
	mesh2.loadModel("..\\res\\monkey3.obj");
	
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag", "");
	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag", "");
	geomShader.init("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");

	texture.init("..\\res\\bricks.jpg"); 
	myCamera.initCamera(glm::vec3(0, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	vector<string> skyboxPaths({ "..\\res\\SkyboxTextures\\right.jpg" ,"..\\res\\SkyboxTextures\\left.jpg" ,"..\\res\\SkyboxTextures\\top.jpg",
									"..\\res\\SkyboxTextures\\bottom.jpg" ,"..\\res\\SkyboxTextures\\front.jpg" ,"..\\res\\SkyboxTextures\\back.jpg" });

	skybox.init(skyboxPaths);
	counter = 0.0f;

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		update();
		drawGame();
	}
}



void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYUP:
				switch (evnt.key.keysym.sym) {
				case SDLK_s:
					// Switch between the different shaders
					isADSEnabled = !isADSEnabled;

					break;
				case SDLK_ESCAPE:
					_gameState = GameState::EXIT;
				default:
					break;
				}
			break;

			// Mouse input
			case SDL_MOUSEMOTION:
				float xRel = evnt.motion.xrel, yRel = evnt.motion.yrel;
				myCamera.Pitch(yRel / 1000);
				myCamera.Yaw(-xRel / 1000);

				break;
		}
	}
	
}

void MainGame::update()
{
	// Calculate the MVP here and then call the link ADS chucking in the MVP


}

void MainGame::linkADS()
{
	glm::vec3 lightPos(20.f, 20.f, 20.f);
	glm::vec3 lightColour(1.f, 1.f, 1.f);
	glm::vec3 objColour(1.f, 1.f, 1.f);

	ADS.setVec3("lightPos", lightPos);
	ADS.setVec3("lightColour", lightColour);
	ADS.setVec3("objectColour", objColour);
	ADS.setVec3("viewPosition", myCamera.getPos());

	glm::mat4 modelMatrix = transform.GetModel();

	ADS.setMat4("model", modelMatrix);

}

void MainGame::linkGeoShader()
{
	float randColX = (float)std::rand() / RAND_MAX;
	float randColY = (float)std::rand() / RAND_MAX;
	float randColZ = (float)std::rand() / RAND_MAX;

	geomShader.setFloat("randColourX", randColX);
	geomShader.setFloat("randColourY", randColY);
	geomShader.setFloat("randColourZ", randColZ);

	geomShader.setFloat("time", counter);

}



void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	skybox.draw(&myCamera);

	transform.SetPos(glm::vec3(0.0,0.0, -20.0));
	transform.SetRot(glm::vec3(0.0,counter * 2, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));

	if (isADSEnabled) {
		ADS.Bind();
		ADS.Update(transform, myCamera);
		linkADS();

	}
	else {
		geomShader.Bind();
		geomShader.Update(transform, myCamera);
		linkGeoShader();
	}

	texture.Bind(0);
	mesh2.draw();
	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
}

