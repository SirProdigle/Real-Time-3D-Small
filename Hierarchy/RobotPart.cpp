#include "RobotPart.h"


std::map<std::string, CommonMesh*> RobotPart::partMeshes = std::map<std::string, CommonMesh*>(); //Define static member

//Also adds mesh to map
void RobotPart::LoadResource(std::string fileName)
{
	//TODO keep a map of string to mesh so we don't load in the same mesh for the smae part, check the map first. If an entry for our mesh exists, use that instead
	this->mesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName.c_str());
	RobotPart::partMeshes.insert( std::pair<std::string,CommonMesh*>(this->partName,this->mesh) ); //Add to map of meshes
}

RobotPart::RobotPart(std::string name, std::string parentName, XMFLOAT4 offset, std::string fileName) {


	//Set props
	this->partName = name;
	this->offset = offset;
	this->offsetMatrix = XMMatrixTranslation(offset.x, offset.y, offset.z);
	this->parentPartName = parentName;
	this->animOffsetMatrix = XMMatrixTranslation(0, 0, 0);

	//Grab mesh from map if already loaded, or load
	if ( RobotPart::partMeshes.find(name) != RobotPart::partMeshes.end())  { 
		this->mesh = RobotPart::partMeshes.at(name);
	}
	else {
		LoadResource("../Resources/Components/" + fileName);
	}
	
}

RobotPart::~RobotPart()
{
	delete this->mesh;
}

void RobotPart::MoveLimb(XMFLOAT3 pos, XMFLOAT3 rot)
{
	animOffsetMatrix = XMMatrixRotationX(XMConvertToRadians(rot.x)) * XMMatrixRotationY(XMConvertToRadians(rot.y)) * XMMatrixRotationZ(XMConvertToRadians(rot.z)) * XMMatrixTranslation(pos.x/10,pos.y/10,pos.z/10);
}

//Root has no mesh
void RobotPart::Draw(XMMATRIX worldMatrix) {
	XMMATRIX finalWorldMatrix = animOffsetMatrix * offsetMatrix;
	RobotPart* currentPart = this;
	while (currentPart->parentPart != nullptr) {
		finalWorldMatrix *= currentPart->parentPart->animOffsetMatrix * currentPart->parentPart->offsetMatrix;
		currentPart = currentPart->parentPart;
	}
	finalWorldMatrix *= worldMatrix;
	finalWorldMatrix = finalWorldMatrix;
	Application::s_pApp->SetWorldMatrix(finalWorldMatrix);
	if(this->mesh != nullptr)
		this->mesh->Draw();
}