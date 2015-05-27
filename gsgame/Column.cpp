#include<glew.h>
#include<DrawUtils.h>
#include "Column.h"
#include<iostream>

Column::Column(float x, float y, int z, int width, int height, GLuint image) : Drawable(y, z) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->image = image;
	this->moving = false;
	this->up = false;
	this->down = false;
	this->left = false;
	this->right = false;
	this->moveTime = .5;
}

Column::~Column() {
}

void Column::move(float deltaTime) {
	if (moving) {
		if (moveTime > 0) {
			float dx = 0;
			float dy = 0;
			if (up) {
				dy -= 16 * (deltaTime / .5);
			}
			else if (down) {
				dy += 16 * (deltaTime / .5);
			}
			else if (left) {
				dx -= 16 * (deltaTime / .5);
			}
			else if (right) {
				dx += 16 * (deltaTime / .5);
			}
			x += dx;
			y += dy;
			moveTime -= deltaTime;
		}
		else {
			x = ((int)(x+8) / 16) * 16;
			y = ((int)(y+8) / 16) * 16;
			moveTime = .5;
			moving = false;
			up = false;
			down = false;
			left = false;
			right = false;
		}
	}
}

void Column::moveUp() {
	moving = true;
	up = true;
}

void Column::moveDown() {
	moving = true;
	down = true;
}

void Column::moveLeft() {
	moving = true;
	left = true;
}

void Column::moveRight() {
	moving = true;
	right = true;
}

void Column::draw(Camera* camera) {
	float camX = camera->getX();
	float camY = camera->getY();
	int drawX = x - (int)camX - 1;
	int drawY = y - (int)camY - 33;
//	if (drawY < 0) drawY--;
//	std::cout << y << "\n";
//	std::cout << drawY << "\n";
	glDrawSprite(image, drawX, drawY, width, height);
}