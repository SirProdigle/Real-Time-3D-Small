#include "Animator.h"
#include "TweenClip.h"
#include <sstream>


Animator::Animator()
{
}


Animator::~Animator()
{
}

void Animator::SetAnimation(std::string file)
{
	if (currentAnimation == nullptr) {
		currentAnimation = new Animation(file);
		return;
	}

	if ( file == this->currentAnimation->name)
		return;

	Animation* anim2 = new Animation(file);


		//Tween
		for (auto it = currentAnimation->Clips.begin(); it != currentAnimation->Clips.end(); it++)
		{
			Clip* currentClip = it->second;
			TweenClip* tweenCurrent = dynamic_cast<TweenClip*>(it->second);
			if (tweenCurrent != nullptr)
				currentClip = tweenCurrent->clip2;
			std::string name = currentClip->GetName();
			TweenClip* tweenClip = new TweenClip(currentClip,anim2->GetClip(name));
			currentClip = tweenClip;
			currentAnimation->Clips.at(name) = tweenClip;
			this->currentAnimation->name = std::to_string(rand() % 500); //random string for tweens
		}
}

void Animator::SetAnimationForParts(std::vector<std::string> partNames, std::string animFile)
{
	Animation* newAnim = new Animation(animFile);
	for (auto it = this->currentAnimation->Clips.begin(); it != currentAnimation->Clips.end(); it++)
	{
		Clip* currentClip = it->second;
		std::string name = currentClip->GetName();
		for (int i = 0; i < partNames.size(); i++)
		{
			if (partNames[i] == name)
				currentClip = newAnim->GetClip(name);
			currentAnimation->Clips.at(name) = currentClip;
		}
	}
}

void Animator::UpdateAnims(float deltaTime)
{
	currentAnimation->UpdateTime(deltaTime);
}

std::pair<XMFLOAT3, XMFLOAT3> Animator::GetOffsetsForPart(std::string robotPart)
{
	return currentAnimation->GetClip(robotPart)->GetOffset();
}


