#pragma once
#include "DEFINITIONS.h"
#include "BaseUserInterfaceElement.h"
#include <functional> 

class UISlider : BaseUserInterfaceElement
{
public:
	UISlider(std::string label, float min, float max, int posX, int posY, int width, int height);

	void setValue(float* v);

	virtual bool updateUI(MouseState& state, int screenHeight);

	virtual void drawUI();

	virtual std::string getType();

	inline void addListener(std::function<void()>func) { listeners.push_back(func); }

	inline float getCurrentValue() { return *currentValue; }

protected:
	float defaultValue;
	float sliderMin, sliderMax;
	float* currentValue;

	bool isDragging;

	std::string sliderLabel;
	GLuint texture;

	void processInteractEvent();
};

