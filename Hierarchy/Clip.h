#pragma once
#include "Application.h"
#include "KeyFrame.h"
class Clip
{
public:
	Clip();
	~Clip();
	void SetName(std::string s) { this->partName = s; }
	std::string GetName() { return partName; }
	virtual void UpdateTime(float delta);
	std::vector<Keyframe> TranslationsX; //All tran and all rot key times the same, only need to read one time from each to move to next keyframe
	std::vector<Keyframe> TranslationsY;
	std::vector<Keyframe> TranslationsZ;
	std::vector<Keyframe> RotationsX;
	std::vector<Keyframe> RotationsY;
	std::vector<Keyframe> RotationsZ;

	virtual std::pair<XMFLOAT3, XMFLOAT3> GetOffset();

	float currentTimeTrans = 0;
	float currentTimeRots = 0;
	int currTran = 0, currRot = 0;
		
protected:
	std::string partName;

	virtual float Tween(float pos1, float ps2, float amount);

};

