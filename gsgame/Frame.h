#pragma once
#include<glew.h>

class Frame
{
	GLuint image;
	int width, height;
public:
	Frame();
	Frame(GLuint image, int width, int height);
	GLuint getImage();
	int getWidth();
	int getHeight();
};

