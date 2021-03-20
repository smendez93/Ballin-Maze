/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/09
----------------------------------------------*/
#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
#include "MapReader.h"
#include <iostream>
#include <fstream>
#include "QuadNode.h"
#include "Wall.h"
#include "Ball.h"
//#include <chrono>

#define DEAD glm::scale(vector3(.01f))*glm::translate(vector3(0.f,-.5f,0.f))


using namespace ReEng; //Using ReEng namespace to use all the classes in the dll

class AppClass : public ReEngAppClass
{
	quaternion m_qOrientation;
	String m_sSelectedObject = "";

	bool render = true; // render quadtree wireframe
	
public:
	typedef ReEngAppClass super;

	std::vector<Wall*> walls;

	Ball* ball;

	MapReader* m_pMapReader;

	QuadNode* m_pMaze;

	int m_iLevelCounter;

	int m_numWallModels;
	int m_usedWallModels;

	vector3 m_v3Rotation;
	matrix4 m_m4Rotation;
	float ROT;
	float MAX_TURN;
	float RECOIL;
	float GRAV_STRENGTH;
	/*
	USAGE: Constructor
	ARGUMENTS:
	- HINSTANCE hInstance -> Instance of the window
	- LPWSTR lpCmdLine -> Command line
	- int nCmdShow -> Number or arguments
	- bool a_bConsole = false -> create command window flag
	OUTPUT: ---
	*/
	AppClass(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow, bool a_bUsingConsole = false) : super(hInstance, lpCmdLine, nCmdShow, a_bUsingConsole) {}
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~AppClass(void) { Release(); }
#ifdef DEBUG
	/*
	USAGE: Reads the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void ReadConfig(void) final {}
	/*
	USAGE: Writes the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void WriteConfig(void) final {}
#endif
	/*
	USAGE: Initialize ReEng variables necessary to create the window
	ARGUMENTS: String a_sWindowName -> Name of the window
	OUTPUT: ---
	*/
	virtual void InitWindow(String a_sWindowName) final;
	/*
	USAGE: Initializes user specific variables, this is executed right after InitApplicationVariables,
	the purpose of this member function is to initialize member variables specific for this lesson
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void InitVariables(void) final;
	/*
	USAGE: Updates the scene
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void Update(void) final;
	/*
	USAGE: Displays the scene
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void Display(void) final;
	/*
	USAGE: Manage the response of key presses
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void ProcessKeyboard(void) final;
	/*
	USAGE: Manage the response of key presses and mouse position
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void ProcessMouse(void) final;
	/*
	USAGE: Releases the application
	IF INHERITED AND OVERRIDEN MAKE SURE TO RELEASE BASE POINTERS (OR CALL BASED CLASS RELEASE)
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void Release(void) final;

	//virtual void AddWall(Wall* wall);

	void ResetBoard();
};

#endif //__APPLICATION_H_
