#include "UIInputField.h"

UIInputField::UIInputField(std::string label, int positionX, int positionY, int width, int height)
	: BaseUserInterfaceElement(positionX, positionY, width, height)
{
	isSelected = false;
	isInputFieldActive = false;
	fieldLabel = label;

	// Generate the texture so we aren't doing it every frame
	glGenTextures(1, &texture);
}

UIInputField::~UIInputField()
{
}

void UIInputField::setText(std::string v)
{
	inputText = std::make_shared<std::string>(v);
}

bool UIInputField::updateUI(MouseState& state, int screenHeight)
{
	BaseUserInterfaceElement::updateUI(state, screenHeight);

	int x = state.mouseXPos;
	int y = screenHeight - state.mouseYPos;

	// If we are inside and we aren't selected, select this UIField
	if (isMouseInside)
	{
		if (state.LeftButtonDown) {
			isSelected = true;
		}
	}
	// Regardless if we are inside or out, if we are selected and there is another left button input, deselect
	else if ((isMouseInside && isSelected) || (!isMouseInside && isSelected)) {
		if (state.LeftButtonDown) {
			isSelected = false;
			isInputFieldActive = false;
		}
	}

	// If we are selected, enable the input field
	if (isSelected && !isInputFieldActive) {
		isInputFieldActive = true;
		processInteractEvent();
	}

	return isInputFieldActive;
}

void UIInputField::drawUI()
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

	// Drawing of the text field's outline when selected
	if (isInputFieldActive)
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
	}

}

std::string UIInputField::getType()
{
	return "inputField";
}

void UIInputField::processInteractEvent()
{
	for (const auto& listener : listeners) {
		listener();
	}
}
