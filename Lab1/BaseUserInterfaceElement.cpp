#include "BaseUserInterfaceElement.h"

std::list<BaseUserInterfaceElement*> BaseUserInterfaceElement::elements;

BaseUserInterfaceElement::BaseUserInterfaceElement(int pX, int pY, int w, int h)
{
	elements.push_back(this);

	posX = pX;
	posY = pY;
	width = w;
	height = h;
	isMouseInside = false;
}

BaseUserInterfaceElement::~BaseUserInterfaceElement()
{
	elements.remove(this);
}

bool BaseUserInterfaceElement::updateUI(MouseState& state, int screenHeight)
{
	int x = state.mouseXPos;
	int y = screenHeight - state.mouseYPos;

	isMouseInside = false;

	// Check to see if the mouse's cursor is inside the element, if so, set the inside to true
	if (x >= posX && x <= posX + width &&
		y >= posY && y <= posY + height)
	{
		isMouseInside = true;
	} 
	return false;
}

BaseUserInterfaceElement* addElement(BaseUserInterfaceElement* element)
{
	static int lastX = 5;
	static int lastY = 5;

	element->setPosition(lastX, lastY);
	lastY += element->getHeight() + 5;
	return element;
}
