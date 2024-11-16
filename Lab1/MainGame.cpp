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
	FBO = new FrameBufferObject();
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
	enviroMappingShader.init("..\\res\\eMapping.vert", "..\\res\\eMapping.frag");
	FBOShader.init("..\\res\\FBOShader.vert", "..\\res\\FBOShader.frag");
	grayScaleShader.init("..\\res\\grayScaleShader.vert", "..\\res\\grayScaleShader.frag");
	edgeDetectionShader.init("..\\res\\edgeDetectionShader.vert", "..\\res\\edgeDetectionShader.frag");

	texture.init("..\\res\\bricks.jpg"); 
	myCamera.initCamera(glm::vec3(0, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	vector<string> skyboxPaths({ "..\\res\\SkyboxTextures\\right.jpg" ,"..\\res\\SkyboxTextures\\left.jpg" ,"..\\res\\SkyboxTextures\\top.jpg",
									"..\\res\\SkyboxTextures\\bottom.jpg" ,"..\\res\\SkyboxTextures\\front.jpg" ,"..\\res\\SkyboxTextures\\back.jpg" });

	skybox.init(skyboxPaths);
	FBO->Init(_gameDisplay.getWidth(), _gameDisplay.getHeight());
	initQuadVAO();

	counter = 0.0f;

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void MainGame::initQuadVAO()
{
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = {
		//positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  1.0f, 0.0f,

		 -1.0f,  1.0f,  0.0f, 1.0f,
		 0.0f, 0.0f,  1.0f, 0.0f,
		 0.0f,  1.0f,  1.0f, 1.0f
	};

	// cube VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


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
	transform.SetPos(glm::vec3(0.0, 0.0, -20.0));
	transform.SetRot(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));

	counter = counter + 0.01f;
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

void MainGame::linkEnviroMapping()
{
	enviroMappingShader.setMat4("model", transform.GetModel());
	enviroMappingShader.setVec3("cameraPos", myCamera.getPos());

	GLuint texLoc = glGetUniformLocation(enviroMappingShader.ID(), "diffuse");
	GLuint skyBoxLoc = glGetUniformLocation(skybox.textureID, "Skybox");

	// Set textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.ID());
	glUniform1i(texLoc, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	glUniform1i(skyBoxLoc, 0);
}



void MainGame::renderFBO()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	edgeDetectionShader.Bind();

	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, FBO->CBO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MainGame::renderMonkey()
{
	texture.Bind(0);
	mesh2.draw();
}

void MainGame::renderSkybox()
{
	skybox.draw(&myCamera);
}

void MainGame::renderActiveShader()
{
	if (isADSEnabled) {
		ADS.Bind();
		ADS.Update(transform, myCamera);
		linkADS();

	}
	else {
		enviroMappingShader.Bind();
		enviroMappingShader.Update(transform, myCamera);
		linkEnviroMapping();
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	FBO->Bind(); // Bind the FBO

	// Render the scene to the FBO
	renderSkybox();
	renderActiveShader();
	renderMonkey();

	FBO->UnBind(); // UnBind and render the FBO
	renderFBO();

	glEnable(GL_DEPTH_TEST);	

	// Render scene again
	renderSkybox();
	renderActiveShader();
	renderMonkey();


	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
}

