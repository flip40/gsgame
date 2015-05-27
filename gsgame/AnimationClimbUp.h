#pragma once
#include "Animation.h"

class AnimationClimbUp : public Animation
{
	bool up;
public:
	void update(float deltaTime);
	void setUp();
	void setDown();
	AnimationClimbUp(float frametime, vector<Frame> frames);
	~AnimationClimbUp();
};

