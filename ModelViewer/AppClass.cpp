#include "AppClass.h"

#define CORNER 3.6f
#define NUM_LEVELS 3

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Model Viewer"); // Window Name
	
	m_v4ClearColor = vector4(0.0f);
}
void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPosition(vector3(0.f, 10.f, 10.f));
	m_sSelectedObject = "";

	ball = new Ball("Ball", vector2(-CORNER, CORNER), BallMat::rubber);

	m_iLevelCounter = 0; // used to increment the level loader

	// load all balls 
	m_pMeshMngr->LoadModel("Ballin\\ball_textured.obj", ball->name + "1");
	m_pMeshMngr->LoadModel("Ballin\\FuzzyBall.obj", ball->name + "2");
	m_pMeshMngr->LoadModel("Ballin\\LeadBall.obj", ball->name + "3");

	ball->name = "Ball1";

	m_pMeshMngr->SetModelMatrix(DEAD, "Ball1");// load under the map
	m_pMeshMngr->SetModelMatrix(DEAD, "Ball2");
	m_pMeshMngr->SetModelMatrix(DEAD, "Ball3");

	m_pMeshMngr->LoadModel("Ballin\\plane_textured.obj", "Plane");
	m_selection = std::pair<int, int>(-1, -1);

	m_v3Rotation = vector3(0.f);
	m_m4Rotation = IDENTITY_M4;

	m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Plane");


	//Enviromental Constants
	ROT = 0.03f;
	MAX_TURN = 0.45f;
	RECOIL = .2f;
	GRAV_STRENGTH = 1.5f;

	//map reader
	m_pMapReader = new MapReader(m_pMeshMngr->GetVertexList("Plane"));

	walls = m_pMapReader->ParseFile("Map" + std::to_string(m_iLevelCounter) + ".txt");

	for (unsigned int i = 0; i < walls.size(); i++)
	{
		if (walls[i]->type != Wall::none)
		{
			m_pMeshMngr->LoadModel("Ballin\\wall_textured.obj", walls[i]->name);
			m_pMeshMngr->SetModelMatrix(walls[i]->m4Transform, walls[i]->name);
		}
	}
	m_numWallModels = walls.size();
	m_pMaze = new QuadNode(walls, 4);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Adding the timer
	static double dTotalTime = 0.0; //Total time of the simulation
	double dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences

	//Update the mesh manager's time
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC)
		CameraRotation();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Camera and Light position 
	quaternion qRotation = glm::quat(m_v3Rotation);

	m_m4Rotation = ToMatrix4(qRotation);
	
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

	//toggle for quadtree
	if (render) {
		m_pMaze->Render();
	}

	m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);

	//Defining the goal
	matrix4 cubeSpot = glm::translate(vector3(CORNER, 0.f, -CORNER))*glm::scale(vector3(.8f,.2f,.8f));
	m_pMeshMngr->AddCubeToRenderList(cubeSpot, REGREEN, SOLID);
	
	//Level progression
	if (ball->position.x > 3.0f && ball->position.z < -3.0f)
	{
		if (m_iLevelCounter == NUM_LEVELS -1)
		{
			m_iLevelCounter = 0;
		}
		else
		{
			m_iLevelCounter++;
		}
		walls.clear();
		walls = m_pMapReader->ParseFile("Map" + std::to_string(m_iLevelCounter) + ".txt");
		for (unsigned int i = 0; i < walls.size(); i++)
		{
			if (walls[i]->type != Wall::none)
			{
				if (i > m_numWallModels)
				{
					m_pMeshMngr->LoadModel("Ballin\\wall_textured.obj", walls[i]->name);
					m_numWallModels++;
				}
				m_pMeshMngr->SetModelMatrix(walls[i]->m4Transform, walls[i]->name);
			}
		}
		m_pMaze = new QuadNode(walls, 4);
		ResetBoard();	// reset board at the end of the mazes
		dTotalTime = 0.0;// reset timer
	}
	
	// UI Elements
	//Controls and Toggles
	m_pMeshMngr->Print("\n");
	m_pMeshMngr->Print("Change Ball Type: ");
	m_pMeshMngr->Print("(1)Rubber, ", REYELLOW);
	m_pMeshMngr->Print("(2)Fuzzy, ", REYELLOW);
	m_pMeshMngr->Print("(3)Lead\n", REYELLOW);
	m_pMeshMngr->Print("Reset: ");
	m_pMeshMngr->Print("(R)\n", REYELLOW); 
	m_pMeshMngr->Print("Tilt: ");
	m_pMeshMngr->Print("Arrow Keys", REYELLOW);
	m_pMeshMngr->Print("\nToggle Quadtee: ");
	m_pMeshMngr->Print("(Q)\n", REYELLOW);
	m_pMeshMngr->Print("Map ");
	m_pMeshMngr->PrintLine(std::to_string(m_iLevelCounter+1), RERED);//Print the map 

	//Timer
	m_pMeshMngr->Print("Time: ");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);// print timer
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

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
	ball = new Ball(ballName, vector2(-CORNER, CORNER), ballMat); // send the ball back to the corner

}

