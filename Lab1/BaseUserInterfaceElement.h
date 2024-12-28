#pragma once
#include <list>
#include <string>

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

	virtual bool updateUI(MouseState &state);
	virtual bool drawUI() = 0;
	
	virtual std::string getType() = 0;

	inline int getWidth() { return width; }
	inline int getHeight() { return height; }

	inline void setPosition(int x, int y) { posX = x; posY = y; }
	inline void setSize(int w, int h) { width = w, height = h; }



	static std::list<BaseUserInterfaceElement*> elements;

protected:

	bool isMouseInside;
	int posX, posY, width, height;
};

BaseUserInterfaceElement* addElement(BaseUserInterfaceElement* element);