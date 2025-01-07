#pragma once
#include "DEFINITIONS.h"

class Texture
{
public:
	Texture() : textureHandler(0) {}

	void Bind(unsigned int unit) const; // bind upto 32 textures
	void init(const std::string& fileName, bool isOneChannel = false);

	~Texture();

	inline GLuint ID() const { return textureHandler; }
protected:
private:

	GLuint textureHandler;
};

