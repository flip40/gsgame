#pragma once
#include"Drawable.h"
#include<glew.h>
#include"Camera.h"

class Column : public Drawable
{
	float x; //y in Drawable, do not place here
	int width, height;
	GLuint image;
	bool moving, up, down, left, right;
	float moveTime;
public:
	Column(float x, float y, int z, int width, int height, GLuint image);
	~Column();
	void draw(class Camera* camera);
	void move(float deltaTime);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
};

