#pragma once
#include "BaseUserInterfaceElement.h"
class UIInputField : public BaseUserInterfaceElement
{
public:
	UIInputField(std::string label, int positionX, int positionY, int width, int height);
	~UIInputField();
	
	void setText(std::string v);

	virtual bool updateUI(MouseState& state, int screenHeight);

	virtual void drawUI();

	virtual std::string getType();

	inline void addListener(std::function<void()>func) { listeners.push_back(func); }


protected:
	void processInteractEvent();

	bool isSelected;
	bool isInputFieldActive;
	std::string fieldLabel;
	std::shared_ptr<std::string> inputText;
	GLuint texture;
};

