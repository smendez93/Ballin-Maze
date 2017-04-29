#pragma once
#include "RE\ReEng.h"
#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Wall.h"
class MapReader
{
	//variables
	const int DIMS = 11; //this is the desired dimensions of the plane. how many "sections" it will be cut into. 
	vector2 dimensionofmodel;

public:
	//constructor
	MapReader(std::vector<vector3> a_lVectorList);

	//this method takes in a list of verticies and figures out the dimensions based on max and min
	//void calculateDimensions(std::vector<vector3> a_lVectorList); //this parameter is the list of verticies
	
	//send the matrix for each wall to the appclass to draw
	//std::vector<matrix4> drawMap();

	std::vector<Wall*> ParseFile(std::string file);

	//destructor
	~MapReader();
};

