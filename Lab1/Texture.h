#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture() : textureHandler(0) {}

	void Bind(unsigned int unit) const; // bind upto 32 textures
	void init(const std::string& fileName);

	~Texture();

	inline GLuint ID() const { return textureHandler; }
protected:
private:

	GLuint textureHandler;
};

