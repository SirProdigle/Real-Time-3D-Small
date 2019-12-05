#pragma once
#include "Application.h"
#include "Clip.h"
#include <map>
#include "RobotPart.h"


class Animation
{

public:
	Animation(std::string DaePath);
	~Animation();
	void UpdateTime(float delta);
	std::map<std::string, Clip*> Clips;
	Clip* GetClip(std::string s) { return Clips.at(s); }
	std::string name;
	
protected:
	
	std::vector<float> SplitFileFloatString(std::string);
};

