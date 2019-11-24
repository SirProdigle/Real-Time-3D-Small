#include "RobotPart.h"

void RobotPart::LoadResource(std::string fileName)
{
	this->mesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName.c_str());
}

RobotPart::RobotPart(std::string name, std::string parentName, XMFLOAT3 offset, std::string fileName) {
	LoadResource("../Resources/Components/" + fileName);
	this->partName = name;
	this->offset = offset;
	this->offsetMatrix = XMMatrixTranslation(offset.x,offset.y,offset.z);
	this->parentPartName = parentName;
}

RobotPart::~RobotPart()
{
	delete this->mesh;
}

//Root has no mesh
void RobotPart::Draw(XMMATRIX worldMatrix) {
	XMMATRIX finalWorldMatrix = offsetMatrix;
	RobotPart* currentPart = this;
	while (currentPart->parentPart != nullptr) {
		finalWorldMatrix += currentPart->parentPart->offsetMatrix;
		currentPart = currentPart->parentPart;
	}
	finalWorldMatrix += worldMatrix;

	Application::s_pApp->SetWorldMatrix(finalWorldMatrix);
	if(this->mesh != nullptr)
		this->mesh->Draw();
}