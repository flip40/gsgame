#include "Frame.h"

Frame::Frame() {
	this->image = 0;
	this->width = 0;
	this->height = 0;
}

Frame::Frame(GLuint image, int width, int height)
{
	this->image = image;
	this->width = width;
	this->height = height;
}

GLuint Frame::getImage() {
	return image;
}

int Frame::getWidth() {
	return width;
}

int Frame::getHeight() {
	return height;
}