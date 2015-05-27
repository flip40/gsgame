#pragma once
#include<queue>

using namespace std;

class AI {
	class Map* map;
	class Character* character;
	class Character* toCharacter;
	queue<AI*>* aiQueue;
	int state;
	float toX, toY;
	float executeTime;
public:
	AI(class Map* map, class Character* character, class Character* toCharacter, queue<AI*>* aiQueue);
	void execute(float deltaTime);
	void setMap(class Map* map);
	void setToCharacter(class Character* toCharacter);
	void decide(); 
private:
	void wander(float deltaTime);
	void goAway(float deltaTime);
	void goTo(float deltaTime);
};

