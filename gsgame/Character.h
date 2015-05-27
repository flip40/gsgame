#pragma once
#include"Drawable.h"
#include<glew.h>
#include<vector>

using namespace std;

class Character : public Drawable {
	float x, prevX, prevY, width, height; //y in Drawable, do not place here
	float velocity; //pixels per second
	int cirleRadius; //characters use a circle for collision centered at x,y
	int currentAnimation;
	bool animating, jumping, pushing, climbing, startingClimb, stoppingClimb;
	bool up, down, left, right;
	float animationTime;
	vector<class Animation*> animations;
public:
	Character(float x, float y, int z, float width, float height, int radius, float velocity, vector<class Animation*> animations);
	void move(bool up, bool down, bool left, bool right, bool mult, float deltaTime, class Map* map);
	void setPos(float x, float y, float z);
	void setPos(float x, float y, class Map* map);
	void revert(); //temporary to handle map collision, I want the character to "bounce" or slide at angles on the map, but this will do for now.
	void draw(class Camera* camera);
	void updateAnimation(bool up, bool down, bool left, bool right, float deltaTime);
	void jump(float deltaTime);
	void push(float deltaTime);
	void startClimbing(float deltaTime);
	void stopClimbing(float deltaTime);
	float getX() const;
	float getY() const;
	float getPrevX() const;
	float getPrevY() const;
	float getWidth() const;
	float getHeight() const;
	float getVelocity() const;
	int getRadius() const;
	bool isClimbing() const;
	bool movingUp() const;
	bool movingDown() const;
	bool movingLeft() const;
	bool movingRight() const;
private:
	bool validPos(float x, float y, class Map* map);
	void climbUp(float deltaTime);
	void climbDown(float deltaTime);
};

bool sortCharByY(const class Character* char1, const class Character* char2);