#pragma once
#include "Clip.h"
class TweenClip :
	public Clip
{
public:
	TweenClip(Clip* c1, Clip* c2) { clip1 = c1; clip2 = c2; transitionTime = 2.0f; currentTransitionTime = 0; this->partName = c1->GetName(); }
	~TweenClip();

	Clip* clip1;
	Clip* clip2;
	float transitionTime;
	float currentTransitionTime;
	virtual void UpdateTime(float delta);
	virtual std::pair<XMFLOAT3, XMFLOAT3> GetOffset();
};

