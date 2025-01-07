#include "UISlider.h"

UISlider::UISlider(std::string label, float min, float max, int posX, int posY, int width, int height)
	: BaseUserInterfaceElement(posX, posY, width, height)
{
	defaultValue = max /2 ;
	currentValue = std::make_shared<float>(defaultValue);

	sliderMin = min; 
	sliderMax = max;
	sliderLabel = label;
	isDragging = false;

	// Generate the texture so we aren't doing it every frame
	glGenTextures(1, &texture);
}

UISlider::~UISlider()
{
}

void UISlider::setValue(float v)
{
	currentValue = std::make_shared<float>(v);
	if (currentValue != NULL) {
		defaultValue = *currentValue;
	}
}

bool UISlider::updateUI(MouseState& state, int screenHeight)
{
	BaseUserInterfaceElement::updateUI(state, screenHeight);

	int x = state.mouseXPos;
	int y = screenHeight - state.mouseYPos;

	// If we are inside and the button is down, we are dragging the slider
	if (isMouseInside) 
	{
		if (state.LeftButtonDown) {
			isDragging = true;
		}
		if (state.RightButtonDown) {
			*currentValue = defaultValue;
		}
	}
	// If we aren't inside but we stop pressing the left button, stop dragging
	if (!state.LeftButtonDown) { isDragging = false; }

	// If we were dragging, set the current value to the position
	if (isDragging) {
		float value = sliderMin + ((float)(x - posX) / width) * (sliderMax - sliderMin);
		*currentValue = value;

		// Clamp the bounds of the current value to the minimum and maximum values
		if (*currentValue > sliderMax) {
			*currentValue = sliderMax;
		}
		else if (*currentValue < sliderMin) {
			*currentValue = sliderMin;
		}
		processInteractEvent();
	}

	return isDragging;
}

void UISlider::drawUI()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

	// Base drawing of the slider's quad
	glBegin(GL_QUADS);
		glVertex2d(posX + width, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + height);
		glVertex2d(posX + width, posY + height);
	glEnd();

	// Drawing of the slider's outline when selected
	if (isDragging)
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
	}
	// Drawing a yellow box if we aren't dragging but hovering
	else if (isMouseInside == true  && isDragging == false)
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
	}
	else
	{
		glColor4f(0.0f, 1.0f, 1.0f, 0.8f);
		glLineWidth(1.0f);
	}
	glBegin(GL_LINE_LOOP);
		glVertex2d(posX + width, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + height);
		glVertex2d(posX + width, posY + height);
	glEnd();

	float currentX = ((*currentValue - sliderMin) / (sliderMax - sliderMin) *(width - 5)) + posX;

	glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
		glVertex2d(currentX + 5, posY);
		glVertex2d(currentX, posY);
		glVertex2d(currentX, posY + height);
		glVertex2d(currentX + 5, posY + height);
	glEnd();

	/*		Text rendering - different to the button.We need two renders.		*/
	// One that doesn't change saying 'Current Value: ' and the other will be updated by the current value

	int yOffset = height + 10.f; // Offsetting the text rendering to 10 pixels plus the height of the slider so it sits just above the slider
	BitmapInfo info = writeText(sliderLabel.c_str(), (width / 2), height, 18);

	// We can't assume that these aren't already enabled/disabled
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	/*	Generating the texture image data using the bitmap data for the text rendering */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.b_w, info.b_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.bitmap);

	/*	Generate the texture that the text will render onto by taking the texture data | We need to flip it as text for some reason rendered upside down */
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		/*		The text ordinarally is flipped along the Y. Thus, we need to flip the texture coords of the square		*/
		// Tex coords - Top Left | Vertex origin of 0,0
		glTexCoord2f(0.0f, 1.0f);
		glVertex2d(posX, posY+ yOffset);

		// Tex coords - Top right | Vertex origin of 1,0
		glTexCoord2f(1.0f, 1.0f);
		glVertex2d(posX + info.b_w, posY+ yOffset);

		// Tex coords - Bottom Right | Vertex origin of 1,1
		glTexCoord2f(1.0f, 0.0f);
		glVertex2d(posX + info.b_w, posY + info.b_h + yOffset);

		// Tex coords - Bottom Left | Vertex origin of 0,1
		glTexCoord2f(0.0f, 0.0f);
		glVertex2d(posX, posY + +info.b_h + yOffset);

	glEnd();
	
	/*			Slider current value display			*/ 
	// Get the previous data for text box allignment
	BitmapInfo preBitmapInfo = info;

	// Convert the float to string for use in text rendering
	std::string stringVal = std::to_string((*currentValue));
	
	info = writeText(stringVal.c_str(), width, height);

	int previousOriginX = posX + preBitmapInfo.b_w;

	// Clearing the texture of the previous text rendering so we can reuse it here
	glBindTexture(GL_TEXTURE_2D, texture);
	glClearTexImage(texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, preBitmapInfo.bitmap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*	Generating the texture image data using the bitmap data for the text rendering */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.b_w, info.b_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.bitmap);

	/*	Generate the texture that the text will render onto by taking the texture data | We need to flip it as text for some reason rendered upside down */
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		/*		The text ordinarally is flipped along the Y. Thus, we need to flip the texture coords of the square		*/
		// Tex coords - Top Left | Vertex origin of 0,0
		glTexCoord2f(0.0f, 1.0f);
		glVertex2d(previousOriginX, posY + yOffset);

		// Tex coords - Top right | Vertex origin of 1,0
		glTexCoord2f(1.0f, 1.0f);
		glVertex2d(previousOriginX + info.b_w, posY + yOffset);

		// Tex coords - Bottom Right | Vertex origin of 1,1
		glTexCoord2f(1.0f, 0.0f);
		glVertex2d(previousOriginX + info.b_w, posY + info.b_h + yOffset);

		// Tex coords - Bottom Left | Vertex origin of 0,1
		glTexCoord2f(0.0f, 0.0f);
		glVertex2d(previousOriginX, posY +info.b_h + yOffset);
	
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);



}

std::string UISlider::getType()
{
	return "slider";
}

void UISlider::processInteractEvent()
{
	for (const auto& listener : listeners) {
		listener();
	}
}

