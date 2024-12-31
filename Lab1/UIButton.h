#pragma once
#include "BaseUserInterfaceElement.h"
class UIButton : BaseUserInterfaceElement
{
public:
	UIButton(std::string label, int positionX, int positionY, int width, int height);

	virtual bool updateUI(MouseState& state, int screenHeight);
	virtual void drawUI();
	virtual std::string getType();

protected:
	bool isDown;
	std::string buttonLabel;
};

