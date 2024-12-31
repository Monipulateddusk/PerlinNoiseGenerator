#pragma once
#include "DEFINITIONS.h"

class FrameBufferObject
{
public:
	GLuint CBO,RBO,FBO; // Colour Buffer, Render Buffer, Frame Buffer

	GLuint quadVAO, quadVBO;

	void Init(const GLsizei width, const GLsizei height);
	void Bind();
	void UnBind();


private:

};

