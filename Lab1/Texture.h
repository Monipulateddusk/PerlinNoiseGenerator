#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();

	void Bind(unsigned int unit); // bind upto 32 textures
	void init(const std::string& fileName);

	~Texture();

	inline GLuint ID() const { return textureHandler; }
protected:
private:

	GLuint textureHandler;
};

