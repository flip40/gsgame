#pragma once
#include "Animation.h"

class AnimationClimbDown : public Animation
{
	bool up;
public:
	void update(float deltaTime);
	void setUp();
	void setDown();
	AnimationClimbDown(float frametime, vector<Frame> frames);
	~AnimationClimbDown();
};

