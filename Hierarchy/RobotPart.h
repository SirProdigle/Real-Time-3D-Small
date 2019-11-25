#pragma once
#include "Application.h"
class Robot; //Forward declare

static std::string MODELFOLDER = "../Resources/Components";

class RobotPart {
	std::string partName;
	RobotPart* parentPart;
	std::string parentPartName;
	XMFLOAT4 offset;
	XMMATRIX offsetMatrix;
	CommonMesh* mesh;

	void LoadResource(std::string fileName);
	void SetParent(RobotPart* p) { parentPart = p; } 
	void Draw(XMMATRIX worldMatrix);
	//Access some part functions for convenience
	friend class Robot;

public:
	RobotPart(std::string name, std::string parentName, XMFLOAT4 offset, std::string fileName);
	~RobotPart();

	

	XMFLOAT4 GetOffset() { return offset; }
	std::string GetPartName() { return partName; }
	RobotPart* GetParent() { return parentPart; }
	std::string GetParentName() { return parentPartName; }
	
	
	
};

