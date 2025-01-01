#include "UISlider.h"

UISlider::UISlider(std::string label, float min, float max, int posX, int posY, int width, int height)
	: BaseUserInterfaceElement(posX, posY, width, height)
{
	defaultValue = 0.0f;
	currentValue = NULL;

	sliderMin = min; 
	sliderMax = max;
	sliderLabel = label;
	isDragging = false;
}

void UISlider::setValue(float* v)
{
	currentValue = v;
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

	float currentX = ((*currentValue - sliderMin) / (sliderMax - sliderMin) *(width - 5)) + posX;

	glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
		glVertex2d(currentX + 5, posY);
		glVertex2d(currentX, posY);
		glVertex2d(currentX, posY + height);
		glVertex2d(currentX + 5, posY + height);
	glEnd();

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
