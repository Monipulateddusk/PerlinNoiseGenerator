#pragma once
#include "BaseUserInterfaceElement.h"
#include <functional> 

class UIButton : BaseUserInterfaceElement
{
public:
	UIButton(std::string label, int positionX, int positionY, int width, int height);

	virtual bool updateUI(MouseState& state, int screenHeight);
	virtual void drawUI();
	virtual std::string getType();
	inline void addListener(std::function<void()> func) { listeners.push_back(func); }

protected:
	bool isDown;
	std::string buttonLabel;

	void processInteractEvent();
};
