#pragma once
#include "DEFINITIONS.h"
#include "stb_truetype.h"

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

struct BitmapInfo {
	unsigned char* bitmap;
	int b_w; // Bitmap width
	int b_h; // Bitmap height
	int l_h; // Line height

	BitmapInfo() {
		bitmap = nullptr;
		b_w = 0; b_h = 0; l_h = 0;
	}

	BitmapInfo(unsigned char* b, int w, int h, int lh) {
		bitmap = b;
		b_w = w;
		b_h = h;
		l_h = lh;
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
	stbtt_fontinfo fontInfo;

	// ChatGPT aided with the syntax for all of this. First time using lambdas which I just wanted a nice and flexible way to handle the events for UI
	std::vector<std::function<void()>> listeners;

	void initFont();
	BitmapInfo writeText(const char* text, int b_w = 512, int b_h = 128, int l_h = 20);

};

BaseUserInterfaceElement* addElement(BaseUserInterfaceElement* element);