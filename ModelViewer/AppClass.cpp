#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Model Viewer"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.0f);
}
void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPosition(vector3(0.f,10.f,10.f));

	m_sSelectedObject = "";

	m_pMeshMngr->LoadModel("Ballin\\ball.obj", "Ball");
	m_pMeshMngr->LoadModel("Ballin\\plane.obj", "Plane");

	m_selection = std::pair<int, int>(-1, -1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	matrix4 m4Position = glm::scale(vector3(.1f))* glm::translate(vector3(0.0f, 1.0f, 0.0f));
	m_v3Rotation = vector3(0.f);
	m_m4Rotation = IDENTITY_M4;

	m_pMeshMngr->SetModelMatrix(m4Position, "Ball");
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Plane");

	ROT = 0.01f;
	MAX_TURN = 0.45f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC)
		CameraRotation();

	ArcBall();

	//if (m_sSelectedObject != "")
	//{
	//	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qOrientation * m_qArcBall), m_sSelectedObject);
	//}

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	quaternion qRotation = glm::quat(m_v3Rotation);

	m_m4Rotation = ToMatrix4(qRotation);
	//m_pMeshMngr->SetModelMatrix(m_m4Rotation, "Ball");
	//m_pMeshMngr->SetModelMatrix(m_m4Rotation, "Plane");
	vector4 camPos = (m_m4Rotation*vector4(0, 8, 0, 1));
	static vector3 lightStart = vector3(4,4,4);
	vector4 lightPos = (m_m4Rotation*vector4(lightStart, 1.f));
	m_pCameraMngr->SetPositionTargetAndView(vector3(camPos.x, camPos.y, camPos.z),vector3(0),-REAXISZ);
	
	m_pLightMngr->SetPosition(vector3(lightPos.x, lightPos.y, lightPos.z));

	static vector3 velocity(0);
	vector4 grav(glm::normalize(-m_pCameraMngr->GetPosition()),1);
	grav = m_m4Rotation*grav;
	velocity += vector3(grav.x*.01f,0, grav.z*.01f);
	m_pMeshMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Ball")*glm::translate(velocity),"Ball");
	
	////Print info on the screen
	//m_pMeshMngr->PrintLine("");//Add a line on top
	//m_pMeshMngr->PrintLine(m_pSystem->GetAppName());
	//m_pMeshMngr->Print("Model: ");
	//m_pMeshMngr->PrintLine(m_sSelectedObject, REGREEN);
	//m_pMeshMngr->Print("Selection: ");
	//m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	//m_pMeshMngr->Print("State: ");
	//if (m_sSelectedObject != "")
	//{
	//	InstanceClass* pInstance = m_pMeshMngr->GetInstanceByName(m_sSelectedObject);
	//	int nState = pInstance->GetCurrentState();
	//	m_pMeshMngr->PrintLine(std::to_string(nState), REGREEN);
	//}
	//else
	//{
	//	m_pMeshMngr->PrintLine("", REGREEN);
	//}
	//m_pMeshMngr->Print("FPS:");
	//m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}
