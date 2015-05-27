#include<glew.h>
#include<DrawUtils.h>
#include"Character.h"
#include<iostream>
#include"Animation.h"
#include"AnimationJump.h"
#include"AnimationClimbUp.h"
#include"AnimationClimbDown.h"

Character::Character(float x, float y, int z, float width, float height, int radius, float velocity, vector<Animation*> animations) : Drawable(y, z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->prevX = x;
	this->prevY = y;
	this->width = width;
	this->height = height;
	this->velocity = velocity;
	this->cirleRadius = radius;
	this->currentAnimation = 0;
	this->animations = animations;
	this->animations[currentAnimation]->setFrame(1);
	this->animating = false;
	this->jumping = false;
	this->up = false;
	this->down = false;
	this->left = false;
	this->right = false;
	this->animationTime = 0;
}

void Character::move(bool up, bool down, bool left, bool right, bool mult, float deltaTime, Map* map) {
	if (animating) {
		return;
	}

	float dx = 0;
	float dy = 0;
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	int multiplier = 2;
	if (mult) {
		multiplier = 1;
	}
	if (up) {
		dy -= (velocity * deltaTime) * multiplier;
	}
	if (down) {
		dy += (velocity * deltaTime) * multiplier;
	}
	if (left) {
		dx -= (velocity * deltaTime) * multiplier;
	}
	if (right) {
		dx += (velocity * deltaTime) * multiplier;
	}
	float tempX = x + dx;
	float tempY = y + dy;
	this->prevX = this->x;
	this->prevY = this->y;
	if (validPos(tempX, y, map)) {
		x = tempX;
	}
	if (validPos(x, tempY, map)) {
		y = tempY;
	}
	//std::cout << "px: " << prevX << ", py: " << prevY << "\n";
	//std::cout << "cx: " << x << ", cy: " << y << "\n";
}

void Character::setPos(float x, float y, float z) {
	this->prevX = this->x;
	this->prevY = this->y;
	this->x = x;
	this->y = y;
	this->z = z;
}

void Character::setPos(float x, float y, Map* map) {
	if (validPos(x, y, map)) {
		this->prevX = this->x;
		this->x = x;
	}
	if (validPos(x, y, map)) {
		this->prevY = this->y;
		this->y = y;
	}
}

void Character::revert() {
	x = prevX;
	y = prevY;
}

void Character::draw(Camera* camera) {
	float camX = camera->getX();
	float camY = camera->getY();
	//HACKY, REPLACE PLEASE!!
	//int drawX = x - (int)camX - (width / 2)/*11*/;
	//int drawY = y - (int)camY - (height * 3 / 4)/*28;*/;
	float drawX;
	float drawY;
	if (width == 8 && height == 10) {
		drawX = x - (int)camX - 4;
		drawY = y - (int)camY - 7;
	}
	else {
		drawX = x - (int)camX - 11;
		drawY = y - (int)camY - 28;
		if (drawX < 0) drawX--;
		if (drawY < 0) drawY--;
	}
	Frame frame = animations[currentAnimation]->getFrame();
	glDrawSprite(frame.getImage(), (int)drawX, (int)drawY, frame.getWidth(), frame.getHeight());
}

void Character::updateAnimation(bool up, bool down, bool left, bool right, float deltaTime) {
	int previousAnimation = currentAnimation;

	if (animating) {
		if (jumping) {
			jump(deltaTime);
		}
		else if (pushing) {
			push(deltaTime);
		}
		else if (startingClimb) {
			startClimbing(deltaTime);
		}
		else if (stoppingClimb) {
			stopClimbing(deltaTime);
		}
		animationTime -= deltaTime;
	}
	else {
		if (climbing) {
			currentAnimation = 14;
		}
		else if (up && !right && !left) {
			currentAnimation = 1;
		}
		else if (down && !right && !left) {
			currentAnimation = 2;
		}
		else if (left && !up && !down) {
			currentAnimation = 3;
		}
		else if (right && !up && !down) {
			currentAnimation = 4;
		}
		else if (up && left) {
			currentAnimation = 5;
		}
		else if (up && right) {
			currentAnimation = 6;
		}
		else if (down && left) {
			currentAnimation = 7;
		}
		else if (down && right) {
			currentAnimation = 8;
		}

		previousAnimation = currentAnimation;

		if (x == prevX && y == prevY) {
			if (climbing) {
				animations[currentAnimation]->setFrame(0);
				return;
			}
			else {
				currentAnimation = 0;
				if (currentAnimation != previousAnimation) {
					animations[currentAnimation]->setFrame(previousAnimation - 1);
					animations[previousAnimation]->reset();
					return;
				}
				else {
					return;
				}
			}
		}
	}

	animations[currentAnimation]->update(deltaTime);

	if (currentAnimation != previousAnimation) {
		animations[previousAnimation]->reset();
	}
}

void Character::jump(float deltaTime) {
	if (animating == false) {
		animating = true;
		animationTime = .5;
	}
	if (jumping == false) {
		jumping = true;
		return;
	}

	if (up) {
		currentAnimation = 9;
	}
	else if (down) {
		currentAnimation = 10;
	}
	else if (left) {
		currentAnimation = 11;
	}
	else if (right) {
		currentAnimation = 12;
	}

	//pre jump
	if (animationTime > .4) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
	//mid jump
	else if (animationTime > .1) {
		float dx = 0;
		float dy = 0;
		if (up) {
			dy -= 31 * (deltaTime / .3);
		}
		else if (down) {
			dy += 31 * (deltaTime / .3);
		}
		else if (left) {
			dx -= 31 * (deltaTime / .3);
		}
		else if (right) {
			dx += 31 * (deltaTime / .3);
		}
		x += dx;
		y += dy;
	}
	//post jump
	else if (animationTime > 0) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
	//end animation
	else {
		animating = false;
		jumping = false;
		if (up) {
			currentAnimation = 1;
		}
		else if (down) {
			currentAnimation = 2;
		}
		else if (left) {
			currentAnimation = 3;
		}
		else if (right) {
			currentAnimation = 4;
		}
		up = false;
		down = false;
		left = false;
		right = false;
		animationTime = 0;
	}

	//cout << animationTime << "\n";
	//cout << "animating: " << animating << " jumping: " << jumping << "\n";
	//cout << "frame: " << animations[currentAnimation].getFrameNum() << " jumping: " << jumping << "\n";
}

void Character::push(float deltaTime) {
	if (animating == false) {
		animating = true;
		animationTime = .5;
	}
	if (pushing == false) {
		pushing = true;
		return;
	}

	if (up) {
		currentAnimation = 1;
	}
	else if (down) {
		currentAnimation = 2;
	}
	else if (left) {
		currentAnimation = 3;
	}
	else if (right) {
		currentAnimation = 4;
	}

	if (animationTime > 0) {
		float dx = 0;
		float dy = 0;
		if (up) {
			dy -= 15 * (deltaTime / .5);
		}
		else if (down) {
			dy += 15 * (deltaTime / .5);
		}
		else if (left) {
			dx -= 15 * (deltaTime / .5);
		}
		else if (right) {
			dx += 15 * (deltaTime / .5);
		}
		x += dx;
		y += dy;
	}
	//end animation
	else {
		animating = false;
		pushing = false;
		if (up) {
			currentAnimation = 1;
		}
		else if (down) {
			currentAnimation = 2;
		}
		else if (left) {
			currentAnimation = 3;
		}
		else if (right) {
			currentAnimation = 4;
		}
		up = false;
		down = false;
		left = false;
		right = false;
		animationTime = 0;
	}

	//cout << animationTime << "\n";
	//cout << "animating: " << animating << " jumping: " << jumping << "\n";
	//cout << "frame: " << animations[currentAnimation].getFrameNum() << " jumping: " << jumping << "\n";
}

void Character::startClimbing(float deltaTime) {
	if (animating == false) {
		animating = true;
		if (up) {
			z++;
			animationTime = .3;
			((AnimationClimbDown*)animations[13])->setUp();
		}
		else if (down) {
			z--;
			animationTime = .4;
			((AnimationClimbUp*)animations[15])->setDown();
		}
	}
	if (startingClimb == false) {
		startingClimb = true;
		return;
	}

	if (up) {
		climbDown(deltaTime);
	}
	else if (down) {
		climbUp(deltaTime);
	}

	//end animation
	if (animationTime <= 0) {
		animating = false;
		startingClimb = false;
		climbing = true;
		currentAnimation = 14;
		animations[currentAnimation]->setFrame(0);
		up = false;
		down = false;
		left = false;
		right = false;
		animationTime = 0;
	}

	//cout << animationTime << "\n";
	//cout << "animating: " << animating << " jumping: " << jumping << "\n";
	//cout << "frame: " << animations[currentAnimation].getFrameNum() << " jumping: " << jumping << "\n";
}

void Character::stopClimbing(float deltaTime) {
	if (animating == false) {
		animating = true;
		if (up) {
			z++;
			animationTime = .4;
			((AnimationClimbUp*)animations[15])->setUp();
		}
		else if (down) {
			z--;
			animationTime = .3;
			((AnimationClimbDown*)animations[13])->setDown();
		}
	}
	if (stoppingClimb == false) {
		stoppingClimb = true;
		return;
	}
	
	if (up) {
		climbUp(deltaTime);
	}
	else if (down) {
		climbDown(deltaTime);
	}

	//end animation
	if (animationTime <= 0) {
		animating = false;
		stoppingClimb = false;
		climbing = false;
		if (up) {
			currentAnimation = 1;
		}
		else if (down) {
			currentAnimation = 2;
		}
		up = false;
		down = false;
		left = false;
		right = false;
		animationTime = 0;
	}

	//cout << animationTime << "\n";
	//cout << "animating: " << animating << " jumping: " << jumping << "\n";
	//cout << "frame: " << animations[currentAnimation].getFrameNum() << " jumping: " << jumping << "\n";
}

float Character::getX() const {
	return x;
}

float Character::getY() const {
	return y;
}

float Character::getPrevX() const {
	return prevX;
}

float Character::getPrevY() const {
	return prevY;
}

float Character::getWidth() const {
	return width;
}

float Character::getHeight() const {
	return height;
}

float Character::getVelocity() const {
	return velocity;
}

int Character::getRadius() const {
	return cirleRadius;
}

bool Character::isClimbing() const {
	return climbing;
}

bool Character::movingUp() const {
	return up;
}

bool Character::movingDown() const {
	return down;
}

bool Character::movingLeft() const {
	return left;
}

bool Character::movingRight() const {
	return right;
}

bool Character::validPos(float x, float y, Map* map) {
	//return map->containsBox(x, y, x + width, y + height);
	return map->isWalkable(x, y, this);
}

void Character::climbUp(float deltaTime) {
	currentAnimation = 15;

	//pre climb
	if (animationTime > .3) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
	//mid climb
	else if (animationTime > .1) {
		float dy = 0;
		if (up) {
			dy -= 32 * (deltaTime / .2);
		}
		else if (down) {
			dy += 32 * (deltaTime / .2);
		}
		y += dy;
	}
	//post climb
	else if (animationTime > 0) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
}

void Character::climbDown(float deltaTime) {
	currentAnimation = 13;

	//pre climb
	if (animationTime > .2) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
	//mid climb
	else if (animationTime > .1) {
		float dy = 0;
		if (up) {
			dy -= 18 * (deltaTime / .1);
		}
		else if (down) {
			dy += 18 * (deltaTime / .1);
		}
		y += dy;
	}
	//post climb
	else if (animationTime > 0) {
		x = x - ((int)x % 16) + 8;
		y = y - ((int)y % 16) + 8;
	}
}

bool sortCharByY(const Character* char1, const Character* char2) {
	if (char1->getY() < char2->getY())
		return true;
	else
		return false;
}