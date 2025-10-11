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

	GLuint texture;
	std::string fieldLabel;

	std::string inputText;
	std::string left;
	std::string right;
	int currentLetter = 0;


	bool isSelected;
	bool isInputFieldActive;


};

