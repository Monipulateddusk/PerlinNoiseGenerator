#pragma once
#include "DEFINITIONS.h"

struct MouseState 
{
	bool LeftButtonDown, RightButtonDown, MiddleButtonDown;
	int mouseXPos, mouseYPos;

	MouseState() {
		LeftButtonDown = false;
		RightButtonDown = false;
		MiddleButtonDown = false;

		mouseXPos = 0;
		mouseYPos = 0;
	}
};

class BaseUserInterfaceElement
{
public:
	BaseUserInterfaceElement(int pX, int pY, int w, int h);
	virtual ~BaseUserInterfaceElement();

	virtual bool updateUI(MouseState& state, int screenHeight);

	virtual void drawUI() = 0;
	
	virtual std::string getType() = 0;

	inline int getWidth() { return width; }
	inline int getHeight() { return height; }

	inline void setPosition(int x, int y) { posX = x; posY = y; }
	inline void setSize(int w, int h) { width = w, height = h; }

	static std::list<BaseUserInterfaceElement*> elements;

protected:

	bool isMouseInside;
	int posX, posY, width, height;

	// ChatGPT aided with the syntax for all of this. First time using lambdas which I just wanted a nice and flexible way to handle the events for UI
	std::vector<std::function<void()>> listeners;
};

BaseUserInterfaceElement* addElement(BaseUserInterfaceElement* element);