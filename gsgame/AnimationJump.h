#pragma once
#include "Animation.h"

class AnimationJump : public Animation
{
public:
	void update(float deltaTime);
	AnimationJump(float frametime, vector<Frame> frames);
	~AnimationJump();
};

