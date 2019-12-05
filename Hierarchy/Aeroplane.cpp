//*********************************************************************************************
// File:			Aeroplane.cpp
// Description:		A very simple class to represent an aeroplane as one object with all the
//					hierarchical components stored internally within the class.
// Module:			Real-Time 3D Techniques for Games
// Created:			Jake - 2010-2011
// Notes:
//*********************************************************************************************

#include "Aeroplane.h"

// Initialise static class variables.
CommonMesh* Aeroplane::s_pPlaneMesh = NULL;
CommonMesh* Aeroplane::s_pPropMesh = NULL;
CommonMesh* Aeroplane::s_pTurretMesh = NULL;
CommonMesh* Aeroplane::s_pGunMesh = NULL;

bool Aeroplane::s_bResourcesReady = false;

Aeroplane::Aeroplane(float fX, float fY, float fZ, float fRotY)
{
	m_mWorldMatrix = XMMatrixIdentity();
	m_mPropWorldMatrix = XMMatrixIdentity();
	m_mTurretWorldMatrix = XMMatrixIdentity();
	m_mGunWorldMatrix = XMMatrixIdentity();
	m_mCamWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0.0f, fRotY, 0.0f, 0.0f);
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);

	m_v4PropOff = XMFLOAT4(0.0f, 0.0f, 1.9f, 0.0f);
	m_v4PropRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4TurretOff = XMFLOAT4(0.0f, 1.05f, -1.3f, 0.0f);
	m_v4TurretRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4GunOff = XMFLOAT4(0.0f, 0.5f, 0.0f, 0.0f);
	m_v4GunRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f;

	m_bGunCam = false;
}

Aeroplane::~Aeroplane(void)
{
}

void Aeroplane::SetWorldPosition(float fX, float fY, float fZ)
{
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);
	UpdateMatrices();
}

void Aeroplane::UpdateMatrices(void)
{
	// Calculate m_mWorldMatrix for plane based on Euler rotation angles and position data.
	// i.e. Use XMMatrixRotationX(), XMMatrixRotationY(), XMMatrixRotationZ() and XMMatrixTranslationFromVector to calculate mRotX, mRotY, mRotZ and mTrans from m_v4Rot
	// Then concatenate the matrices to calculate m_mWorldMatrix
	XMMATRIX mRotX, mRotY, mRotZ, mTrans;
	XMMATRIX mPlaneCameraRot, mForwardMatrix;
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4Rot.z));
	mTrans = XMMatrixTranslation(m_v4Pos.x, m_v4Pos.y, m_v4Pos.z);

	m_mWorldMatrix = mRotZ * mRotX * mRotY * mTrans;


	// Calculate m_mPropWorldMatrix for propeller based on Euler rotation angles and position data.
	// Parent the propeller to the plane
	XMMATRIX mpRotX, mpRotY, mpRotZ, mpTrans;
	mpRotX = XMMatrixRotationX(XMConvertToRadians(m_v4PropRot.x));
	mpRotY = XMMatrixRotationY(XMConvertToRadians(m_v4PropRot.y));
	mpRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4PropRot.z));
	mpTrans = XMMatrixTranslation(m_v4PropOff.x, m_v4PropOff.y, m_v4PropOff.z);

	m_mPropWorldMatrix = mpRotX * mpRotY * mpRotZ * mpTrans * m_mWorldMatrix;


	// Calculate m_mTurretWorldMatrix for propeller based on Euler rotation angles and position data.
	// Parent the turret to the plane
	XMMATRIX mtRotX, mtRotY, mtRotZ, mtTrans;
	mtRotX = XMMatrixRotationX(XMConvertToRadians(m_v4TurretRot.x));
	mtRotY = XMMatrixRotationY(XMConvertToRadians(m_v4TurretRot.y));
	mtRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4TurretRot.z));
	mtTrans = XMMatrixTranslation(m_v4TurretOff.x, m_v4TurretOff.y, m_v4TurretOff.z);

	m_mTurretWorldMatrix = mtRotX * mtRotY * mtRotZ * mtTrans * m_mWorldMatrix;


	// Calculate m_mGunWorldMatrix for gun based on Euler rotation angles and position data.
	// Parent the gun to the turret
	XMMATRIX mgRotX, mgRotY, mgRotZ, mgTrans;
	mgRotX = XMMatrixRotationX(XMConvertToRadians(m_v4GunRot.x));
	mgRotY = XMMatrixRotationY(XMConvertToRadians(m_v4GunRot.y));
	mgRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4GunRot.z));
	mgTrans = XMMatrixTranslation(m_v4GunOff.x, m_v4GunOff.y, m_v4GunOff.z);

	m_mGunWorldMatrix = mgRotX * mgRotY * mgRotZ * mgTrans * m_mTurretWorldMatrix;


	// Calculate m_mCameraWorldMatrix for camera based on Euler rotation angles and position data.
	XMMATRIX mcRotX, mcRotY, mcRotZ, mcTrans;
	mcRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mcRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mcRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));
	mcTrans = XMMatrixTranslation(m_v4CamOff.x, m_v4CamOff.y, m_v4CamOff.z);

	XMMATRIX planeFollowMatrix = mRotY * mTrans; //Only follow Y rotation of Plane
	XMMATRIX gunFollowMatrix = mgRotY * mgTrans; //Only follow Y rotation of Gun


	m_vForwardVector = m_mWorldMatrix.r[2]; // grab forwad vector (the plane's z axis)



	//Switch between parenting the camera to the plane (without X and Z rotations) and the gun based on m_bGunCam
	if (m_bGunCam)
		m_mCamWorldMatrix = mcRotX * mcRotY * mcRotZ * mcTrans * planeFollowMatrix;
	else
		m_mCamWorldMatrix = mcRotX * mcRotY * mcRotZ * mcTrans * gunFollowMatrix;

	// Get the camera's world position (m_vCamWorldPos) out of m_mCameraWorldMatrix
	//DECOMPOSE MATRIX IN TO POSITION DATA
	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;
}

void Aeroplane::Update(bool bPlayerControl)
{
	// DON'T DO THIS UNTIL YOu HAVE COMPLETED THE FUNCTION ABOVE
	if (bPlayerControl)
	{
		//Up/Down pitch and return if key not held down
		if (Application::s_pApp->IsKeyPressed('Q') && m_v4Rot.x > -60) {
			m_v4Rot.x -= 1;
		}
		else if (Application::s_pApp->IsKeyPressed('A') && m_v4Rot.x < 60 && m_fSpeed >= 0.5) { //Additional "takeoff" speed required
			m_v4Rot.x += 1;
		}
		else if (m_v4Rot.x < 0) {
			m_v4Rot.x += 1;
		}
		else if (m_v4Rot.x > 0) {
			m_v4Rot.x -= 0.5f;
		}

		if (Application::s_pApp->IsKeyPressed('O')) {
			if (m_v4Rot.z < 20) {
				m_v4Rot.z += 10;
			}
			m_v4Rot.y -= 0.3f;
		}
		else if (Application::s_pApp->IsKeyPressed('P')) {
			if (m_v4Rot.z > -20) {
				m_v4Rot.z -= 10;
			}
			m_v4Rot.y += 0.3f;
		}
		else if (m_v4Rot.z < 0) {
			m_v4Rot.z += 0.5f;
			m_v4Rot.y -= 0.1f;
		}
		else if (m_v4Rot.z > 0) {
			m_v4Rot.z -= 0.5f;
			m_v4Rot.y += 0.1f;
		}


	} // End of if player control

	// Apply a forward thrust and limit to a maximum speed of 1
	m_fSpeed += 0.001f;

	if (m_fSpeed > 1)
		m_fSpeed = 1;

	// Rotate propeller and turret
	m_v4PropRot.z += 100 * m_fSpeed;
	m_v4TurretRot.y += 0.1f;

	// Tilt gun up and down as turret rotates
	m_v4GunRot.x = (sin((float)XMConvertToRadians(m_v4TurretRot.y * 4.0f)) * 10.0f) - 10.0f;

	UpdateMatrices();

	// Move Forward
	if (!Application::planeStop) {
		XMVECTOR vCurrPos = XMLoadFloat4(&m_v4Pos);
		vCurrPos += m_vForwardVector * m_fSpeed;
		XMStoreFloat4(&m_v4Pos, vCurrPos);
	}
}

void Aeroplane::LoadResources(void)
{
	s_pPlaneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "../Resources/Plane/plane.x");
	s_pPropMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "../Resources/Plane/prop.x");
	s_pTurretMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "../Resources/Plane/turret.x");
	s_pGunMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "../Resources/Plane/gun.x");
}

void Aeroplane::ReleaseResources(void)
{
	delete s_pPlaneMesh;
	delete s_pPropMesh;
	delete s_pTurretMesh;
	delete s_pGunMesh;
}

void Aeroplane::Draw(void)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	s_pPlaneMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mPropWorldMatrix);
	s_pPropMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mTurretWorldMatrix);
	s_pTurretMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mGunWorldMatrix);
	s_pGunMesh->Draw();
}
