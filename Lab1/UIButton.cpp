#include "UIButton.h"

UIButton::UIButton(std::string label, int positionX, int positionY, int width, int height)
	: BaseUserInterfaceElement(positionX, positionY, width, height)
{
	buttonLabel = label;
	isDown = false;
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
		glColor4f(0.0f, 1.0f, 1.0f, 0.8f);
		glLineWidth(1.0f);
	}
	glBegin(GL_LINE_LOOP);
		glVertex2d(posX + width, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + height);
		glVertex2d(posX + width, posY + height);
	glEnd();

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
