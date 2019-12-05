#include "Robot.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

Robot::Robot(std::string fileName, XMMATRIX worldMatrix )
{
	this->animator = new Animator();
	robotWorldMatrix = worldMatrix;
	this->CreateParts(fileName); //read file and create robot parts

	//Loop through all parts and set their parents
	for (auto it = robotParts.begin(); it != robotParts.end(); it++)
	{
		RobotPart* currentPart = it->second;
		//If parent then set it
		if (!currentPart->parentPartName.empty()) {
			RobotPart* parent = robotParts.at(currentPart->GetParentName());
			currentPart->SetParent(parent);
		}
		else {
			currentPart->SetParent(nullptr);
		}
	}
}


void Robot::CreateParts(std::string fileName) {
	//Read the hierarchy file and process all the robot part's into our map
	std::ifstream hierarchyFile(fileName);
	while (hierarchyFile.is_open() && !hierarchyFile.eof()) {
		if (hierarchyFile.is_open()) {
			//Treat the file as chunks of 3 lines (1 robot part)
			std::string partName;
			std::string parentName;
			std::string offsetString;

			std::getline(hierarchyFile, partName);
			std::getline(hierarchyFile, parentName);
			std::getline(hierarchyFile, offsetString);

			//Cut the \" \" surrounding the strings
			partName = partName.substr(1, partName.length() - 2);
			parentName = parentName.substr(1, parentName.length() - 2);

			//process the line into a float3
			XMFLOAT4 offset = GetOffsetFromLine(offsetString);

			//Create the part and add it it our map with partName identifier
			RobotPart* part = new RobotPart(partName, parentName, offset, partName + ".x");
			this->robotParts.insert(std::pair<std::string, RobotPart*>(partName, part));
		}
	}
	hierarchyFile.close();

}

//Continually shorten our string cutting off one float&comma at a time
XMFLOAT4 Robot::GetOffsetFromLine(std::string line) {
	int xStringSplit = line.find_first_of(',');
	std::string xString = line.substr(0, xStringSplit);

	line = line.substr(xStringSplit + 1);

	int yStringSplit = line.find_first_of(',');
	std::string yString = line.substr(0, yStringSplit);

	line = line.substr(yStringSplit + 1);

	int zStringSplit = line.find_first_of(',');
	std::string zString = line.substr(0, zStringSplit);

	//seems to work but possibly a more accurate way to do this
	double x = std::stod(xString);
	double y = std::stod(yString);
	double z = std::stod(zString);

	return XMFLOAT4(x/10, y/10, z/10, 1); //Divide by 10 for technical reasoning

}

void Robot::Draw() {
	Application::s_pApp->SetWorldMatrix(robotWorldMatrix);
	for (auto it = robotParts.begin(); it != robotParts.end(); it++)
	{
		it->second->Draw(robotWorldMatrix);
	}
}

void Robot::Animate()
{
	float updateAmount = 0.01f; 
	if (Application::slowMo)
		updateAmount *= 0.1f;
	if (animator->IsPaused())
		return; //Don't animate if paused
	else {
		animator->UpdateAnims(updateAmount);
		for (auto it = robotParts.begin(); it != robotParts.end(); it++)
		{
			RobotPart* currentPart =  it->second;
			std::string name = currentPart->GetPartName();
			auto offsets = animator->GetOffsetsForPart(name);
			currentPart->MoveLimb(offsets.first, offsets.second);
		}
	}
}

Robot::~Robot()
{
	//Loop through all our parts in the robotParts map and destruct them
	for (auto it = robotParts.begin(); it != robotParts.end(); it++)
	{
		delete it->second;
	}
}
