#include "Application.h"
#include "Heightmap.h"
#include "Aeroplane.h"
#include "Robot.h"
#include "Animation.h"

Application* Application::s_pApp = NULL;
bool Application::slowMo = false;
const int CAMERA_MAP = 0;
const int CAMERA_PLANE = 1;
const int CAMERA_GUN = 2;
const int CAMERA_MAX = 3;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool Application::HandleStart()
{

	

	s_pApp = this;

	this->SetWindowTitle("Liam Warner b6018527");

	m_bWireframe = false;
	m_Robot = new Robot("../Resources/hierarchy.txt", XMMatrixTranslation(0,2.5,0));
	m_Robot2 = new Robot("../Resources/hierarchy.txt", XMMatrixTranslation(20.0f, 2.5, 0));
	m_Robot3 = new Robot("../Resources/hierarchy.txt", XMMatrixTranslation(-40.0f, 2.5, 0));
	m_Robot->animator->SetAnimation("../Resources/Maya Files/RobotAttackAnim.dae");
	m_Robot2->animator->SetAnimation("../Resources/Maya Files/RobotIdleAnim.dae");
	m_pHeightMap = new HeightMap("../Resources/heightmap.bmp", 2.0f);
	m_pAeroplane = new Aeroplane(0.0f, 3.5f, 0.0f, 105.0f);

	m_pAeroplane->LoadResources();

	m_cameraZ = 50.0f;
	m_rotationAngle = 0.f;

	if(!this->CommonApp::HandleStart())
		return false;

	this->SetRasterizerState(false, m_bWireframe);

	m_cameraState = CAMERA_MAP;

	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleStop()
{
	delete m_pHeightMap;
	Aeroplane::ReleaseResources();
	delete m_pAeroplane;
	delete m_Robot;

	this->CommonApp::HandleStop();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleUpdate()
{
	if (this->IsKeyPressed(VK_F1)) {
		m_cameraState = CAMERA_MAP;
	}
	if (this->IsKeyPressed(VK_F2)){
		m_cameraState = CAMERA_GUN;
	}
	if (this->IsKeyPressed('S')) {
		if (Application::slowMo == false)
			Application::slowMo = true;
		else Application::slowMo = false;
	}
	if (this->IsKeyPressed('1')) {
		if (this->IsKeyPressed(VK_UP)) {
			std::vector<std::string> parts{"root", "body", "left_shoulder", "left_elbow", "left_wrist", "right_shoulder", "right_elbow", "right_wrist", "neck"};
			m_Robot->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotAttackAnim.dae");
			m_Robot2->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotAttackAnim.dae");
		}
		else if (this->IsKeyPressed(VK_DOWN)) {
			std::vector<std::string> parts{"pelvis","left_hip","right_hip","left_ankle","right_ankle","left_knee","right_knee" };
			m_Robot->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotAttackAnim.dae");
			m_Robot2->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotAttackAnim.dae");
		}
		else {
			m_Robot->animator->SetAnimation("../Resources/Maya Files/RobotAttackAnim.dae");
			m_Robot2->animator->SetAnimation("../Resources/Maya Files/RobotAttackAnim.dae");
		}
	}
	if (this->IsKeyPressed('2')) {
		if (this->IsKeyPressed(VK_UP)) {
			std::vector<std::string> parts{ "root", "body", "left_shoulder", "left_elbow", "left_wrist", "right_shoulder", "right_elbow", "right_wrist", "neck" };
			m_Robot->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotIdleAnim.dae");
			m_Robot2->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotIdleAnim.dae");
		}
		else if (this->IsKeyPressed(VK_DOWN)) {
			std::vector<std::string> parts{ "pelvis","left_hip","right_hip","left_ankle","right_ankle","left_knee","right_knee" };
			m_Robot->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotIdleAnim.dae");
			m_Robot2->animator->SetAnimationForParts(parts, "../Resources/Maya Files/RobotIdleAnim.dae");
		}
		else {
			m_Robot->animator->SetAnimation("../Resources/Maya Files/RobotIdleAnim.dae");
			m_Robot2->animator->SetAnimation("../Resources/Maya Files/RobotIdleAnim.dae");
		}
	}

		m_Robot->Animate();
		m_Robot2->Animate();



		m_rotationAngle += .0f;

		if (m_cameraState == CAMERA_MAP)
		{
			if (this->IsKeyPressed('Q'))
				m_cameraZ -= 2.0f;

			if (this->IsKeyPressed('A'))
				m_cameraZ += 2.0f;
		}

		static bool dbC = false;

		if (this->IsKeyPressed('C'))
		{
			if (!dbC)
			{
				if (++m_cameraState == CAMERA_MAX)
					m_cameraState = CAMERA_MAP;

				dbC = true;
			}
		}
		else
		{
			dbC = false;
		}

		static bool dbW = false;
		if (this->IsKeyPressed('W'))
		{
			if (!dbW)
			{
				m_bWireframe = !m_bWireframe;
				this->SetRasterizerState(false, m_bWireframe);
				dbW = true;
			}
		}
		else
		{
			dbW = false;
		}

		m_pAeroplane->Update(m_cameraState != CAMERA_MAP);
	}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleRender()
{
	XMFLOAT3 vUpVector(0.0f, 1.0f, 0.0f);
	XMFLOAT3 vCamera, vLookat;

	switch(m_cameraState)
	{
		case CAMERA_MAP:
			vCamera = XMFLOAT3(sin(m_rotationAngle) * m_cameraZ, m_cameraZ / 4, cos(m_rotationAngle) * m_cameraZ);
			vLookat = XMFLOAT3(0.0f, 4.0f, 0.0f);
			break;
		case CAMERA_PLANE:
			m_pAeroplane->SetGunCamera(false);
			vCamera = XMFLOAT3(m_pAeroplane->GetCameraPosition().x, m_pAeroplane->GetCameraPosition().y, m_pAeroplane->GetCameraPosition().z);
			vLookat = XMFLOAT3(m_pAeroplane->GetFocusPosition().x, m_pAeroplane->GetFocusPosition().y, m_pAeroplane->GetFocusPosition().z);
			break;
		case CAMERA_GUN:
			m_pAeroplane->SetGunCamera(true);
			vCamera = XMFLOAT3(m_pAeroplane->GetCameraPosition().x, m_pAeroplane->GetCameraPosition().y, m_pAeroplane->GetCameraPosition().z);
			vLookat = XMFLOAT3(m_pAeroplane->GetFocusPosition().x, m_pAeroplane->GetFocusPosition().y, m_pAeroplane->GetFocusPosition().z);
			break;
	}

	XMMATRIX matView;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&vCamera), XMLoadFloat3(&vLookat), XMLoadFloat3(&vUpVector));

	XMMATRIX matProj;
	matProj = XMMatrixPerspectiveFovLH(float(XM_PI / 4), 2, 1.5f, 5000.0f);

	this->SetViewMatrix(matView);
	this->SetProjectionMatrix(matProj);

	this->EnablePointLight(0, XMFLOAT3(100.0f, 100.f, -100.f), XMFLOAT3(1.f, 1.f, 1.f));
	this->SetLightAttenuation(0, 200.f, 2.f, 2.f, 2.f);
	this->EnableDirectionalLight(1, XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT3(0.65f, 0.55f, 0.65f));

	this->Clear(XMFLOAT4(.2f, .2f, .6f, 1.f));

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	this->SetWorldMatrix(matWorld);

	m_pHeightMap->Draw();
	m_Robot->Draw();
	m_Robot2->Draw();
	m_Robot3->Draw();
	m_pAeroplane->Draw();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application application;

	Run(&application);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
