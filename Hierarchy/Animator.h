#pragma once
#include "Application.h"
#include "Animation.h"


class Animator
{
public:
	Animator();
	~Animator();
	void Play() { paused = false; }
	void Pause() { paused = true; }
	void SetAnimation(std::string file); //Load to current

	void SetAnimationForParts(std::vector<std::string> partNames, std::string animFile);
	bool IsPaused() { return paused; }
	void UpdateAnims(float deltaTime); //keyframes on all clips
	std::pair<XMFLOAT3 ,XMFLOAT3> GetOffsetsForPart(std::string robotPart);


private:
	bool paused = false;
	Animation* currentAnimation = nullptr;
	float currTime = 0;

};

