#pragma once
#include "RobotPart.h"
#include "Application.h"
#include <map>

class Robot
{
	XMMATRIX robotWorldMatrix;
	std::map<std::string, RobotPart* > robotParts;

	XMFLOAT4 GetOffsetFromLine(std::string line);




public:
	Robot(std::string fileName, XMMATRIX world);

	void CreateParts(std::string fileName);
	void Draw();

	~Robot();
};

