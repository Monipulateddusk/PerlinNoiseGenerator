#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <iostream>
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

