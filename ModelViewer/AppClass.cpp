#include "AppClass.h"

#define CORNER 3.6f

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
	m_pCameraMngr->SetPosition(vector3(0.f, 10.f, 10.f));
	m_sSelectedObject = "";

	ball = new Ball("Ball", vector2(-CORNER,CORNER), BallMat::rubber);

	m_pMeshMngr->LoadModel("Ballin\\ball_textured.obj", ball->name + "1");
	m_pMeshMngr->LoadModel("Ballin\\FuzzyBall.obj", ball->name + "2");
	m_pMeshMngr->LoadModel("Ballin\\LeadBall.obj", ball->name + "3");

	ball->name = "Ball1";

	m_pMeshMngr->SetModelMatrix(DEAD, "Ball1");
	m_pMeshMngr->SetModelMatrix(DEAD, "Ball2");
	m_pMeshMngr->SetModelMatrix(DEAD, "Ball3");

	m_pMeshMngr->LoadModel("Ballin\\plane_textured.obj", "Plane");
	m_selection = std::pair<int, int>(-1, -1);

	m_v3Rotation = vector3(0.f);
	m_m4Rotation = IDENTITY_M4;

	m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Plane");
	
	

	ROT = 0.03f;
	MAX_TURN = 0.45f;
	RECOIL = .2f;
	GRAV_STRENGTH = 1.5f;

	//map reader
	m_pMapReader = new MapReader(m_pMeshMngr->GetVertexList("Plane"));

	walls = m_pMapReader->ParseFile("Map.txt");

	for (unsigned int i = 0; i < walls.size(); i++)
	{
		if (walls[i]->type != Wall::none)
		{
			m_pMeshMngr->LoadModel("Ballin\\wall_textured.obj", walls[i]->name);
			m_pMeshMngr->SetModelMatrix(walls[i]->m4Transform, walls[i]->name);
		}
	}
	m_pMaze = new QuadNode(walls, 4);
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
	
	vector4 camPos = (m_m4Rotation*vector4(0, 16, 0, 1));
	vector3 lightStart = vector3(4, 4, 4);
	vector4 lightPos = (m_m4Rotation*vector4(lightStart, 1.f));

	m_pCameraMngr->SetPositionTargetAndView(vector3(camPos.x, camPos.y, camPos.z), vector3(0), -REAXISZ);

	m_pLightMngr->SetPosition(vector3(lightPos.x, lightPos.y, lightPos.z));

	ball->Update(m_m4Rotation);
	vector3 rebound = vector3(0.f);
	if (m_pMaze->CheckCollision(ball, rebound)) {
		//perform reflection of velocity
		vector3 rejection = -(ball->velocity-(ball->velocity-(rebound*glm::dot(ball->velocity, rebound)/(glm::dot(rebound,rebound)))));

		ball->velocity += rejection; //zeroes relative to normal
		ball->velocity += rejection*ball->material.bounce; // rebound based on bounce factor;

	}
	
	m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);

	matrix4 cubeSpot = glm::translate(vector3(CORNER, 0.f, -CORNER))*glm::scale(vector3(.8f,.2f,.8f));
	m_pMeshMngr->AddCubeToRenderList(cubeSpot, REGREEN, SOLID);

	m_pMeshMngr->Print("\n");
	m_pMeshMngr->Print("Change Ball Type: ");
	m_pMeshMngr->Print("(1)Rubber, ", REYELLOW);
	m_pMeshMngr->Print("(2)Fuzzy, ", REYELLOW);
	m_pMeshMngr->Print("(3)Lead\n", REYELLOW);
	m_pMeshMngr->Print("Reset: ");
	m_pMeshMngr->Print("(R)\n", REYELLOW);
	m_pMeshMngr->Print("Tilt: ");
	m_pMeshMngr->Print("Arrow Keys\n", REYELLOW);

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
	SafeDelete(m_pMapReader);
	walls.clear();
}

void AppClass::ResetBoard()
{
	m_v3Rotation = vector3(0.f);
	m_m4Rotation = IDENTITY_M4;
	std::string ballName = ball->name;
	BallMaterial ballMat = ball->material;
	delete ball;
	ball = new Ball(ballName, vector2(-CORNER, CORNER), ballMat);

}
//
//void AppClass::AddWall(Wall * wall)
//{
//	walls.push_back(wall);
//}
