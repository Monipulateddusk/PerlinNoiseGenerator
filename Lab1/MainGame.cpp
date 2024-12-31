#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
					Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
					Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)) };

unsigned int indices[] = { 0, 1, 2 };



MainGame::MainGame()
{
	counter = 0; isADSEnabled = false;
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	FBO = new FrameBufferObject();
	perlinNoiseSeedValue = 0;
	elementSelected = NULL;
	
}

MainGame::~MainGame()
{
	delete monkey;

	for (auto it = BaseUserInterfaceElement::elements.begin(); it != BaseUserInterfaceElement::elements.end(); )
	{
		if (*it != nullptr) {
			std::cout << "Processing element at iterator: " << *it << std::endl;

			*it = nullptr;
		}
		// Erase the element and move to the next
		it = BaseUserInterfaceElement::elements.erase(it);

		// Optionally print the size of the list after each erase
		std::cout << "Remaining elements in list: " << BaseUserInterfaceElement::elements.size() << std::endl;
	}

	
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
	/*mesh2.loadModel("..\\res\\monkey3.obj");*/

	monkey = new GameObject("..\\res\\monkey3.obj", "..\\res\\bricks.jpg");

	lavaTexture.init("..\\res\\lava3.jpg");
	noiseTexture.init("..\\res\\noise.png");


	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag", "");
	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag", "");
	geomShader.init("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");
	enviroMappingShader.init("..\\res\\eMapping.vert", "..\\res\\eMapping.frag");
	FBOShader.init("..\\res\\FBOShader.vert", "..\\res\\FBOShader.frag");
	grayScaleShader.init("..\\res\\grayScaleShader.vert", "..\\res\\grayScaleShader.frag");
	edgeDetectionShader.init("..\\res\\edgeDetectionShader.vert", "..\\res\\edgeDetectionShader.frag");
	noiseShader.init("..\\res\\noise.vert", "..\\res\\noise.frag");
	glowShader.init("..\\res\\glow.vert", "..\\res\\glow.frag");
	

	/*texture.init("..\\res\\bricks.jpg"); */
	myCamera.initCamera(glm::vec3(0, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	vector<string> skyboxPaths({ "..\\res\\SkyboxTextures\\right.jpg" ,"..\\res\\SkyboxTextures\\left.jpg" ,"..\\res\\SkyboxTextures\\top.jpg",
									"..\\res\\SkyboxTextures\\bottom.jpg" ,"..\\res\\SkyboxTextures\\front.jpg" ,"..\\res\\SkyboxTextures\\back.jpg" });

	skybox.init(skyboxPaths);
	FBO->Init(_gameDisplay.getWidth(), _gameDisplay.getHeight());
	initQuadVAO();

	counter = 0.0f;

	noiseGen.CreatePerlinNoiseTexture();

	UIButton* button = new UIButton("Button", _gameDisplay.getWidth() /2 , _gameDisplay.getHeight() / 2, 200, 50);

	// Uncomment if we are doing camera movement
	// SDL_SetRelativeMouseMode(SDL_TRUE);
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
	SDL_GetMouseState(&mouseState.mouseXPos, &mouseState.mouseYPos);

	mouseState.LeftButtonDown == SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	mouseState.MiddleButtonDown == SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	mouseState.RightButtonDown == SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);

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


				case SDLK_RIGHT: // Increment the seed value on the perlin noise generator
					perlinNoiseSeedValue++;
					break;

				case SDLK_LEFT: // Decrement the seed value on the perlin noise generator
					perlinNoiseSeedValue == 0 ? perlinNoiseSeedValue = 0 : perlinNoiseSeedValue--;
					break;

				case SDLK_BACKSPACE:
					system("cls");
					std::cout << "BACKSPACE KEY PRESSED" << std::endl;
					noiseGen.SetSeedValue(perlinNoiseSeedValue);
					noiseGen.CreatePerlinNoiseTexture();
					break;

				case SDLK_ESCAPE:
					_gameState = GameState::EXIT;
				default:
					break;
				}
			break;

			// Mouse input
			// Uncomment if we are doing camera movement
			//case SDL_MOUSEMOTION:
			//	float xRel = evnt.motion.xrel, yRel = evnt.motion.yrel;
			//	myCamera.Pitch(yRel / 1000);
			//	myCamera.Yaw(-xRel / 1000);

			//	break;

		}
	}
	//system("cls");
	//std::cout << "Mouse Position: (" << mouseState.mouseXPos << ", " << mouseState.mouseYPos << ")\n";
	//std::cout << "Left Button Down: " << (mouseState.LeftButtonDown ? "Yes" : "No") << "\n";
	//std::cout << "Middle Button Down: " << (mouseState.MiddleButtonDown ? "Yes" : "No") << "\n";
	//std::cout << "Right Button Down: " << (mouseState.RightButtonDown ? "Yes" : "No") << "\n";
}

void MainGame::update()
{
	monkey->transform.SetPos(glm::vec3(0.0, 0.0, -20.0));
	monkey->transform.SetRot(glm::vec3(0.0, counter * 2, 0.0));
	monkey->transform.SetScale(glm::vec3(1.0, 1.0, 1.0));

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

	glm::mat4 modelMatrix = monkey->transform.GetModel();

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
	enviroMappingShader.setMat4("model", monkey->transform.GetModel());
	enviroMappingShader.setVec3("cameraPos", myCamera.getPos());

	GLuint texLoc = glGetUniformLocation(enviroMappingShader.ID(), "diffuse");
	GLuint skyBoxLoc = glGetUniformLocation(skybox.textureID, "Skybox");

	// Set textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, monkey->texture.ID());
	glUniform1i(texLoc, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	glUniform1i(skyBoxLoc, 0);
}

void MainGame::linkNoiseShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	noiseShader.Bind();
	noiseShader.setFloat("time", counter / 20);
	noiseShader.setFloat("fogDensity", 0.5);
	noiseShader.setVec3("fogColor", 0.0f, 0.0f, 0.5f);
	noiseShader.setFloat("maxDist", 10.0f);
	noiseShader.setFloat("minDist", 0.0f);
	GLuint t1L = glGetUniformLocation(noiseShader.ID(), "texture1"); //texture 1 location
	GLuint t2L = glGetUniformLocation(noiseShader.ID(), "texture2");

	//set textures
	glActiveTexture(GL_TEXTURE0); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, noiseTexture.ID());
	glUniform1i(t1L, 0);

	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, lavaTexture.ID());
	glUniform1i(t2L, 1);

	//type of and texture to bind to unit


	monkey->transform.SetPos(glm::vec3(0.0, 0.0, -25));
	monkey->transform.SetRot(glm::vec3(0.0, 10, 0.0));
	monkey->transform.SetScale(glm::vec3(1.2, 1.2, 1.2));

	myCamera.MoveRight(0.0001);

	noiseShader.Update(monkey->transform, myCamera);



	monkey->mesh.draw();
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
	monkey->texture.Bind(0);
	monkey->mesh.draw();
}

void MainGame::renderSkybox()
{
	skybox.draw(&myCamera);
}

void MainGame::renderActiveShader()
{
	if (isADSEnabled) {
		ADS.Bind();
		ADS.Update(monkey->transform, myCamera);
		linkADS();

	}
	else {
		enviroMappingShader.Bind();
		enviroMappingShader.Update(monkey->transform, myCamera);
		linkEnviroMapping();
	}
}

void MainGame::drawUIElements()
{
	// Basically, only allow processing of a specific UI element so that we check if the cursor is within the bounds of an object and then process it's logic
	for (auto it = BaseUserInterfaceElement::elements.begin(); it != BaseUserInterfaceElement::elements.end(); it++) 
	{
		BaseUserInterfaceElement* element = (*it);

		element->drawUI();
		// If we have our mouse hovered over something, process it and only it
		if (element->updateUI(mouseState, _gameDisplay.getHeight())) {

			if (elementSelected == nullptr) {
				elementSelected = element;
			}
		}
		// When our mouse is not over this object, deselect it but it was before, deselect it. 
		else if (element == elementSelected) {
			elementSelected = nullptr;
		}
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(1.0f, 1.0f, 1.0f, 1.0f);

	// ----- 3D Rendering -----
	// Enable depth, texture culling and texture mapping for 3D rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	renderSkybox();
	linkNoiseShader();
	renderActiveShader();
	renderMonkey();

	// ----- 2D Rendering -----
	
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Clear any active shaders
	glUseProgram(0); // Disable shaders

	// Disable 3D-specific settings
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Set up an orthographic projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, _gameDisplay.getWidth(), 0.0, _gameDisplay.getHeight(), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	drawUIElements();

	// Restore matrices
	glPopMatrix();               // Restore modelview matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();               // Restore projection matrix
	glMatrixMode(GL_MODELVIEW);

	// Swap the buffer
	_gameDisplay.swapBuffer();
}



