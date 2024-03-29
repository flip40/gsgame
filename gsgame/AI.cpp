#include "AI.h"
#include "Character.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>

AI::AI(Map* map, Character* character, Character* toCharacter, queue<AI*>* aiQueue) {
	this->map = map;
	this->character = character;
	this->toCharacter = toCharacter;
	this->state = 0;
	this->aiQueue = aiQueue;
	this->executeTime = 0;
	this->waitTime = 0;
	aiQueue->push(this);
	//std::cout << aiQueue->size() << "\n";
}

void AI::execute(float deltaTime) {
	executeTime += deltaTime;
	if (state != 4 && executeTime > 1) {
		state = 0;
		executeTime = 0;
		aiQueue->push(this);
	}
	
	// save coords for later use
	float prevX = character->getX();
	float prevY = character->getY();
	
	switch (state) {
	case 1: wander(deltaTime);
//		std::cout << "wandering" << "\n";
		break;
	case 2: goTo(deltaTime);
//		std::cout << "go to" << "\n";
		break;
	case 3: goAway(deltaTime);
//		std::cout << "go away" << "\n";
		break;
	case 4: wait(deltaTime);
//		std::cout << "waiting" << "\n";
		break;
	}
	
	character->updateAnimationByPrevCoords(prevX, prevY, deltaTime);
}

void AI::wander(float deltaTime) {
	//std::cout << "wandering" << "\n";
	//vector from char to point
	float dx = toX - character->getX();
	float dy = toY - character->getY();
	//modify vector based on slope
	float dx2 = dx / (abs(dx) + abs(dy));
	float dy2 = dy / (abs(dx) + abs(dy));
	//apply velocity to vecotrs
	float velocity = character->getVelocity() * deltaTime;
	float dx3 = dx2 * velocity;
	float dy3 = dy2 * velocity;
	if (abs(dx) - abs(dx3) < 0 || abs(dy) - abs(dy3) < 0) {
		character->setPos(toX, toY, map);
		character->setPos(toX, toY, map); // sets prevX and prevY to same values
//		aiQueue->push(this);
		this->waitTime = rand() % 5;
		state = 4;
	}
	else {
		character->setPos(character->getX() + dx3, character->getY() + dy3, map);
	}
}

void AI::goAway(float deltaTime) {
	//std::cout << "going to char" << "\n";
	//vector from char to point
	float dx = toX - character->getX();
	float dy = toY - character->getY();
	//modify vector based on slope
	float dx2 = dx / (abs(dx) + abs(dy));
	float dy2 = dy / (abs(dx) + abs(dy));
	//apply velocity to vecotrs
	float velocity = character->getVelocity() * deltaTime;
	float dx3 = dx2 * velocity;
	float dy3 = dy2 * velocity;
	if (abs(dx) - abs(dx3) < 0 || abs(dy) - abs(dy3) < 0) {
		aiQueue->push(this);
		state = 0;
	}
	else {
		character->setPos(character->getX() - dx3, character->getY() - dy3, map);
	}
}

void AI::goTo(float deltaTime) {
	//std::cout << "going to char" << "\n";
	//vector from char to point
	float dx = toX - character->getX();
	float dy = toY - character->getY();
	//modify vector based on slope
	float dx2 = dx / (abs(dx) + abs(dy));
	float dy2 = dy / (abs(dx) + abs(dy));
	//apply velocity to vecotrs
	float velocity = character->getVelocity() * deltaTime;
	float dx3 = dx2 * velocity;
	float dy3 = dy2 * velocity;
	if (abs(dx) - abs(dx3) < 0 || abs(dy) - abs(dy3) < 0) {
		character->setPos(toX, toY, map);
		aiQueue->push(this);
		state = 0;
	}
	else {
		character->setPos(character->getX() + dx3, character->getY() + dy3, map);
	}
}

void AI::wait(float deltaTime) {
	if (this->waitTime < 0) {
		aiQueue->push(this);
		state = 0;
	} else {
		this->waitTime -= deltaTime;
	}
}

void AI::setMap(Map* map) {
	this->map = map;
}

void AI::setToCharacter(Character* toCharacter) {
	this->toCharacter = toCharacter;
}

void AI::decide() {
	//std::cout << "deciding" << "\n";
	float distance = sqrt(pow(toCharacter->getX() - character->getX(), 2) + pow(toCharacter->getY() - character->getY(), 2));
	if (distance > 32) {
		//wander
		state = 1;
		if (character->getX() > 16) {
			toX = character->getX() + (rand() % 32 - 16);
		}
		else {
			toX = character->getX() + (rand() % 32 - character->getX());
		}

		if (character->getY() > 16) {
			toY = character->getY() + (rand() % 32 - 16);
		}
		else {
			toY = character->getY() + (rand() % 32 - character->getY());
		}

		aiQueue->pop();
	}
	else /* if (distance <= 32) */ {
		//run away
		toX = toCharacter->getX();
		toY = toCharacter->getY();
		aiQueue->pop();
		state = 3;
	}
}
