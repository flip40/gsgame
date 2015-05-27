#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "Camera.h"
#include "Drawable.h"
#include "Character.h"
#include "Map.h"
#include "AI.h"

using namespace std;

class Game
{
	//contain information relevant to the current gamestate (level, map, characters, drawables, etc.)
	string level;
	class Camera* camera;
	class Character* character;
	class Map* map;
	vector<class Drawable*> drawables;
	vector<class Character*> characters;
	queue<class AI*> aiQueue;
public:
	Game(string saveFile);
	void load(string levelDir);
	void save();
};