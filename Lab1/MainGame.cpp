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
	delete cube;
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	monkey = new GameObject("..\\res\\monkey3.obj", "..\\res\\PerlinNoise\\GeneratedPerlinNoise.png");
	cube = new GameObject("..\\res\\cube.obj", "..\\res\\PerlinNoise\\GeneratedPerlinNoise.png", true);
	plane = new GameObject("..\\res\\subdividedPlane2.obj", "..\\res\\PerlinNoise\\GeneratedPerlinNoise.png");
	plane->transform.SetPos(glm::vec3(0.0, -0.5f, -25));
	plane->transform.SetScale(glm::vec3(1.2, 1.2, 1.2));

	sphere = new GameObject("..\\res\\sphere.obj", "..\\res\\bricks.jpg");

	generatedPerlinNoiseTexture.init("..\\res\\PerlinNoise\\GeneratedPerlinNoise.png", true);


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
	heightMapShader.init("..\\res\\heightMap.vert", "..\\res\\heightMap.frag");
	

	myCamera.initCamera(glm::vec3(-1, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	vector<string> skyboxPaths({ "..\\res\\SkyboxTextures\\right.jpg" ,"..\\res\\SkyboxTextures\\left.jpg" ,"..\\res\\SkyboxTextures\\top.jpg",
									"..\\res\\SkyboxTextures\\bottom.jpg" ,"..\\res\\SkyboxTextures\\front.jpg" ,"..\\res\\SkyboxTextures\\back.jpg" });

	skybox.init(skyboxPaths);
	FBO->Init(_gameDisplay.getWidth(), _gameDisplay.getHeight());
	initQuadVAO();

	counter = 0.0f;

	noiseGen.CreatePerlinNoiseTexture();
	initUI();
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
/// <summary>
/// All UI elements are put into a vector containing smart pointers to each UI element. 
/// Due to polymorphism, we can correctly store it as the base component and get their overriden mechanics
/// 
/// Method is:
/// -	Declare object as shared pointer
/// -	Declare the Lambda event
/// -	Add the shared pointer to the vector
/// 
/// OpenAI assisted with getting the lambda event declaration working syntaxically
/// /// </summary>
void MainGame::initUI()
{
	// Had odd occourance where the pointer given by the static inside the baseUserInterfaceElement was causing corrupted pointers when some events tried to fire
	// Converting to smart pointers seemed to fix it
	int origin = (_gameDisplay.getWidth() / 3) * 1.81;
	int yOrigin = (_gameDisplay.getHeight());


	/*	Seed Slider	*/
	yOrigin -= 70;
	std::shared_ptr<UISlider> seedSlider = std::make_shared<UISlider>("Seed", 1.0f, 10000.0f, false, origin, yOrigin, 400, 20);
	seedSlider->setValue(noiseGen.getSeedValue());
	seedSlider->addListener([seedSlider, this]()
		{
			unsigned int seed = seedSlider->getCurrentValue();
			noiseGen.setSeedValue(seed);
		}
	);
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(seedSlider));

	/*	Amplitude Slider	*/
	yOrigin -= 70;
	std::shared_ptr<UISlider> ampSlider = std::make_shared<UISlider>("Amplitude", 1.0f, 6.0f, true, origin, yOrigin, 400, 20);
	ampSlider->setValue(noiseGen.getAmpCount());
	ampSlider->addListener([ampSlider, this]()
		{
			float ampCount = ampSlider->getCurrentValue();
			noiseGen.setAmpCount(ampCount);
		}
	);
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(ampSlider));

	/*	Frequency Slider	*/
	yOrigin -= 70;
	std::shared_ptr<UISlider> freqSlider = std::make_shared<UISlider>("Frequency", 1.0f, 3.00f, true, origin, yOrigin, 400, 20);
	freqSlider->setValue(noiseGen.getFreqCount());
	freqSlider->addListener([freqSlider, this]()
		{
			float freqCount = freqSlider->getCurrentValue();
			noiseGen.setFreqCount(freqCount);
		}
	);
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(freqSlider));

	/*	Ocative Slider	*/
	yOrigin -= 70;
	std::shared_ptr<UISlider> octSlider = std::make_shared<UISlider>("Ocative Count", 1.0f, 16.0f, false, origin, yOrigin, 400, 20);
	octSlider->setValue(noiseGen.getOcativeCount());
	octSlider->addListener([octSlider, this]()
		{
			int octCount = octSlider->getCurrentValue();
			noiseGen.setOcativeCount(octCount);
		}
	);
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(octSlider));

	/*	Generate Perlin Button	*/
	yOrigin -= 120;
	origin += 104.f;
	std::shared_ptr<UIButton> button = std::make_shared<UIButton>("Show Sphere", origin, yOrigin, 200, 50);
	button->addListener([]() {std::cout << "EVENT WHOOO!!!" << std::endl;});
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(button));

	/*	Show Different Model Button	*/
	yOrigin -= 90;
	button = std::make_shared<UIButton>("Generate Perlin", origin, yOrigin, 200, 50);
	button->addListener([this]() 
		{
			noiseGen.CreatePerlinNoiseTexture();
			setPerlinNoiseTexture();
		}
	);
	uiElements.push_back(std::static_pointer_cast<BaseUserInterfaceElement>(button));
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

void MainGame::activateOrtho()
{
	// Switching the matrix mode to projection, saving it and resetting it to identity to remove translations.
	// Done for both projection and model view matrixs
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, _gameDisplay.getWidth(), 0.0, _gameDisplay.getHeight(), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void MainGame::disableOrtho()
{
	// Restoring the projection and model view matrixs for use next frame in 3d rendering 
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void MainGame::processInput()
{
	SDL_Event evnt;
	SDL_GetMouseState(&mouseState.mouseXPos, &mouseState.mouseYPos);

	mouseState.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	mouseState.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	mouseState.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);

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
		}
	}
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
	if(isADSEnabled){ glBindTexture(GL_TEXTURE_2D, noiseTexture.ID()); }
	else{ glBindTexture(GL_TEXTURE_2D, generatedPerlinNoiseTexture.ID()); }

	glUniform1i(t1L, 0);

	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, lavaTexture.ID());
	glUniform1i(t2L, 1);

	//type of and texture to bind to unit


	monkey->transform.SetPos(glm::vec3(0.0, 0.0, -25));
	monkey->transform.SetRot(glm::vec3(90, 0, 90));
	monkey->transform.SetScale(glm::vec3(1.2, 1.2, 1.2));

	noiseShader.Update(monkey->transform, myCamera);



	monkey->mesh.draw();
}

void MainGame::linkHeightMapShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE); // Allowing faces that don't face the camera to be rendered
	heightMapShader.Bind();

	GLuint hML = glGetUniformLocation(heightMapShader.ID(), "heightMap");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, generatedPerlinNoiseTexture.ID());
	glUniform1i(hML, 1);

	heightMapShader.setMat4("transform", plane->transform.GetModel());

	heightMapShader.Update(plane->transform, myCamera);

	plane->transform.SetRot(glm::vec3(0, 0+counter, 25.1f));

	plane->mesh.draw();
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

void MainGame::setPerlinNoiseTexture()
{
	generatedPerlinNoiseTexture.ClearTexture();
	generatedPerlinNoiseTexture.init("..\\res\\PerlinNoise\\GeneratedPerlinNoise.png", true);
}

void MainGame::drawBackgroundUI()
{
	// Draw background
	// If the origin is the bottom left, the origin of the quad would be 2/3 into the total screen size
	int origin = (_gameDisplay.getWidth() / 3) * 1.8f;
	glColor4f(0.3f, 0.3f, 0.3f, 1.f);

	// Base drawing of the button's quad
	glBegin(GL_QUADS);
		glVertex2d(_gameDisplay.getWidth(), 0);
		glVertex2d(origin, 0);
		glVertex2d(origin, _gameDisplay.getHeight());
		glVertex2d(_gameDisplay.getWidth(), _gameDisplay.getHeight());
	glEnd();
}

void MainGame::drawUIElements()
{
	// Basically, only allow processing of a specific UI element so that we check if the cursor is within the bounds of an object and then process it's logic
	for (auto& element : uiElements)
	{
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

void MainGame::drawGeneratedPerlinNoise()
{
	int originX = ((_gameDisplay.getWidth() / 3) * 1.8f) + 76;
	int originY = 10;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, generatedPerlinNoiseTexture.ID());

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		// Tex coords - Top Left | Vertex origin of 0,0
		glTexCoord2f(0.0f, 1.0f);
		glVertex2d(originX, originY);

		// Tex coords - Top right | Vertex origin of 1,0
		glTexCoord2f(1.0f, 1.0f);
		glVertex2d(originX + 256, originY);

		// Tex coords - Bottom Right | Vertex origin of 1,1
		glTexCoord2f(1.0f, 0.0f);
		glVertex2d(originX + 256, originY + 256);

		// Tex coords - Bottom Left | Vertex origin of 0,1
		glTexCoord2f(0.0f, 0.0f);
		glVertex2d(originX, originY + 256);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(1.0f, 1.0f, 1.0f, 1.0f);

	// ----- 3D Rendering -----
	// Enable depth, texture culling and texture mapping for 3D rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	
	renderSkybox();
	//linkNoiseShader();
	//renderActiveShader();
	linkHeightMapShader();

	// For whatever reason we need to render some kind of shader for the text to render.
	// If we render none, the text wont render despite the fact we are disabling shaders in the UI
	enviroMappingShader.Bind();
	linkEnviroMapping();
	// ----- 2D Rendering -----
	
	// Clear depth buffer & disabling 3D-specific effects or settings including disabling shaders
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(0);

	// Set up an orthographic projection matrix
	activateOrtho();

	drawBackgroundUI();
	drawUIElements();
	drawGeneratedPerlinNoise();

	// Disable said orthographic matrix for 3d rendering next frame
	disableOrtho();

	// Swap the buffer
	_gameDisplay.swapBuffer();
}



