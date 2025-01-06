#include "UIButton.h"

UIButton::UIButton(std::string label, int positionX, int positionY, int width, int height)
	: BaseUserInterfaceElement(positionX, positionY, width, height)
{
	buttonLabel = label;
	isDown = false;

	// Generate the texture so we aren't doing it every frame
	glGenTextures(1, &texture);
}

bool UIButton::updateUI(MouseState& state, int screenHeight)
{
	BaseUserInterfaceElement::updateUI(state, screenHeight);

	// Usual procedure, if the mouse is inside the element and they click, process the button
	if (isMouseInside) {
		if (state.LeftButtonDown) 
		{
			isDown = true;
		}
		// If it was down, raise it back up and return true
		else if (isDown) {
			isDown = false;
			processInteractEvent();
			return true;
		}
	}
	return false;
}

void UIButton::drawUI()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f,0.8f);

	// Base drawing of the button's quad
	glBegin(GL_QUADS);
		glVertex2d(posX + width,	posY);
		glVertex2d(posX,			posY);
		glVertex2d(posX,			posY + height);
		glVertex2d(posX + width,	posY + height);
	glEnd();

	// Drawing of the button's outline when selected
	if (isMouseInside == true) 
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glLineWidth(1.0f);
	}
	glBegin(GL_LINE_LOOP);
		glVertex2d(posX + width, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + height);
		glVertex2d(posX + width, posY + height);
	glEnd();

	// Render text. Use the function in the base class converting the text to bitmap. 
	// Use that bitmap to render to a texture and then make that texture's position map the button
	BitmapInfo info = writeText(buttonLabel.c_str(), width, height, 18);

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
		/*	OpenAI aided with the syntax on how to bind the texture to the bounds of the button		*/
		// Tex coords - Top Left | Vertex origin of 0,0
		glTexCoord2f(0.0f, 1.0f);
		glVertex2d(posX, posY);

		// Tex coords - Top right | Vertex origin of 1,0
		glTexCoord2f(1.0f, 1.0f);
		glVertex2d(posX + width, posY);

		// Tex coords - Bottom Right | Vertex origin of 1,1
		glTexCoord2f(1.0f, 0.0f);
		glVertex2d(posX + width, posY + height);

		// Tex coords - Bottom Left | Vertex origin of 0,1
		glTexCoord2f(0.0f, 0.0f);
		glVertex2d(posX, posY + height);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}

std::string UIButton::getType()
{
	return "button";
}

void UIButton::processInteractEvent()
{
	for (const auto& listener : listeners) {
		listener();
	}
}
