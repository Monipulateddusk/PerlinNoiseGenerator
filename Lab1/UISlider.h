#pragma once
#include "DEFINITIONS.h"
#include "BaseUserInterfaceElement.h"
#include <functional> 

class UISlider : BaseUserInterfaceElement
{
public:
	UISlider(std::string label, float min, float max, int posX, int posY, int width, int height);
	~UISlider();
	void setValue(float v);

	virtual bool updateUI(MouseState& state, int screenHeight);

	virtual void drawUI();

	virtual std::string getType();

	inline void addListener(std::function<void()>func) { listeners.push_back(func); }

	inline float getCurrentValue() 
	{	
		if (currentValue != nullptr && currentValue) 
		{ 
			return *currentValue; 
		}	
		else {
			std::cerr << "Error: currentValue is null!" << std::endl;
			return 0.0f; // Default value
		}
	}

protected:
	float defaultValue;
	float sliderMin, sliderMax;
	std::shared_ptr<float> currentValue;

	bool isDragging;

	std::string sliderLabel;
	GLuint texture;

	void processInteractEvent();
};

