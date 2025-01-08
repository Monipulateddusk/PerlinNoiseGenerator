#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

void Texture::init(const std::string& fileName, bool isOneChannel)
{
	channel = 4;
	if (isOneChannel) { channel = 3; }
	int width, height, numComponents; //width, height, and no of components of image

	std::cout << "Loading image with file path: " << fileName << std::endl;
	m_ImageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, channel); //load the image and store the data

	if (m_ImageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
	}

	if(textureHandler == NULL){
		glGenTextures(1, &textureHandler); // number of and address of textures
	}
	glBindTexture(GL_TEXTURE_2D, textureHandler); //bind texture - define type 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	GLint format = isOneChannel ? GL_RGB : GL_RGBA;

	if(isOneChannel){
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, m_ImageData); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Image Data
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, m_ImageData); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Image Data
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandler); // number of and address of textures
	
	stbi_image_free(m_ImageData);
	m_ImageData = nullptr;
}

void Texture::ClearTexture()
{
	// Determine format
	GLint format = 0;
	if (channel == 3) {	format = GL_RGB;}
	else if (channel == 4) { format = GL_RGBA; }
	else { assert("UNDETERMINED FORMAT WHEN CLEARING TEXTURE OF ID: ", ID()); }

	glClearTexImage(textureHandler, 0, format, GL_UNSIGNED_BYTE, m_ImageData); // Clearing the data so we can write to the same texture ID but with new texture
}

void Texture::Bind(unsigned int unit) const
{
	assert(unit >= 0 && unit <= 31); /// check we are working with one of the 32 textures

	glActiveTexture(GL_TEXTURE0 + unit); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, textureHandler); //type of and texture to bind to unit
}
