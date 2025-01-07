#pragma once
#include "DEFINITIONS.h"
#include "BaseUserInterfaceElement.h"
#include <functional> 

class UISlider : public BaseUserInterfaceElement
{
public:
	UISlider(std::string label, float min, float max, bool isFloat ,int posX, int posY, int width, int height);
	~UISlider();
	void setValue(float v);

	virtual bool updateUI(MouseState& state, int screenHeight);

	virtual void drawUI();

	virtual std::string getType();

	inline void addListener(std::function<void()>func) { listeners.push_back(func); }

	inline float getCurrentValue() { return *currentValue; };

protected:
	float defaultValue;
	float sliderMin, sliderMax;
	std::shared_ptr<float> currentValue;

	bool isDragging, isFloatValue;

	std::string sliderLabel;
	GLuint texture;

	void processInteractEvent();
};

