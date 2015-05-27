#pragma once
#include<glew.h>
#include<vector>
#include"Frame.h"

using namespace std;

class Animation
{
protected:
	vector<Frame> animationFrames;
	float frameTime;
	float currentTime;
	int currentFrame;
public:
	Animation(float frameTime, vector<Frame> frames);
	virtual void update(float deltaTime);
	void setFrame(int frame);
	void reset();
	class Frame getFrame();
	int getFrameNum();
	~Animation();
};

