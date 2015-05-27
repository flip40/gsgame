#include "Animation.h"
#include<iostream>

Animation::Animation(float frametime, vector<Frame> frames) {
	this->animationFrames = frames;
	this->frameTime = frametime;
	currentTime = frametime;
	currentFrame = 0;
}

void Animation::update(float deltatime) {
	currentTime = currentTime - deltatime;
	if (currentTime < 0)
	{
		currentTime = frameTime; 
		if (currentFrame < animationFrames.size()-1)
		{
			currentFrame++;
		}
		else
		{
			currentFrame = 0;
		}
	}
//	cout << frameTime << "\n";
//	cout << currentTime << "\n";
//	cout << deltatime << "\n";
//	cout << currentFrame << "\n";
}

void Animation::setFrame(int frame) {
	currentFrame = frame;
}

void Animation::reset()
{
	currentFrame = 0;
	currentTime = frameTime;
}

Frame Animation::getFrame()
{
	//cout << "frames:" << animationFrames.size() << "current: " << currentFrame << "\n";
	return animationFrames[currentFrame];
}

int Animation::getFrameNum() {
	return currentFrame;
}

Animation::~Animation()
{

}
