#pragma once
#include "DEFINITIONS.h"

class Texture
{
public:
	Texture() : textureHandler(0), channel(0), m_ImageData(nullptr){}

	void Bind(unsigned int unit) const; // bind upto 32 textures
	void init(const std::string& fileName, bool isOneChannel = false);

	~Texture();

	inline GLuint ID() const { return textureHandler; }

	void ClearTexture();
protected:
private:
	int channel;
	unsigned char* m_ImageData; // Holds image data so texture can be cleared if needed
	GLuint textureHandler;
};

