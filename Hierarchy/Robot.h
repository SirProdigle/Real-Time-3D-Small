#pragma once
#include "RobotPart.h"
#include "Application.h"
#include "Animator.h"
#include <map>

class Robot
{
	XMMATRIX robotWorldMatrix;
	std::map<std::string, RobotPart* > robotParts;

	XMFLOAT4 GetOffsetFromLine(std::string line);



public:
	Robot(std::string fileName, XMMATRIX world);
	Animator* animator;
	void CreateParts(std::string fileName);
	void Draw();
	void Animate();

	~Robot();
};

