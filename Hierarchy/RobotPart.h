#pragma once
#include "Application.h"
#include <map>
class Robot; //Forward declare

static std::string MODELFOLDER = "../Resources/Components";

class RobotPart {
	std::string partName;
	RobotPart* parentPart;
	std::string parentPartName;
	XMFLOAT4 offset;
	XMMATRIX offsetMatrix;
	XMMATRIX animOffsetMatrix;
	CommonMesh* mesh;

	void LoadResource(std::string fileName);
	void SetParent(RobotPart* p) { parentPart = p; } 
	void Draw(XMMATRIX worldMatrix);
	//Access some part functions for convenience
	friend class Robot;

public:
	RobotPart(std::string name, std::string parentName, XMFLOAT4 offset, std::string fileName);
	~RobotPart();

	static std::map<std::string, CommonMesh*> partMeshes; 

	XMFLOAT4 GetOffset() { return offset; }
	std::string GetPartName() { return partName; }
	RobotPart* GetParent() { return parentPart; }
	std::string GetParentName() { return parentPartName; }
	void MoveLimb(XMFLOAT3 pos, XMFLOAT3 rot);
	
	
	
};

