#include "MapReader.h"

MapReader::MapReader(std::vector<vector3> a_lVectorList)
{
	//initializing variables
	desireddimensions = 12;

	//reading in the map file
	FileReaderClass reader;
	reader.ReadFile("map.txt");
	for(int i = 0; i < desireddimensions; i++) 
	{
		//reading each line and saving it to filecontent
		reader.ReadNextLine(false);
		filecontent += reader.m_sLine;
	}
	reader.CloseFile();

	calculateDimensions(a_lVectorList);
}

void MapReader::calculateDimensions(std::vector<vector3> a_lVectorList)
{
	//get the max and the min
	//Count the points of the incoming list
	uint nVertexCount = a_lVectorList.size();

	//If there are none just return, we have no information to create the BS from
	if (nVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	vector3 m_v3Max;
	vector3 m_v3Min;

	//Get the max and min out of the list 
	//only need the x and the z for this. the y doesn't matter
	for (uint nVertex = 1; nVertex < nVertexCount; nVertex++)
	{
		if (m_v3Min.x > a_lVectorList[nVertex].x) //If min is larger than current
			m_v3Min.x = a_lVectorList[nVertex].x;
		else if (m_v3Max.x < a_lVectorList[nVertex].x)//if max is smaller than current
			m_v3Max.x = a_lVectorList[nVertex].x;

		if (m_v3Min.z > a_lVectorList[nVertex].z) //If min is larger than current
			m_v3Min.z = a_lVectorList[nVertex].z;
		else if (m_v3Max.z < a_lVectorList[nVertex].z)//if max is smaller than current
			m_v3Max.z = a_lVectorList[nVertex].z;
	}

	//get the dimensions based on the max and min by finding the difference
	dimensionofmodel = vector2(m_v3Max.x - m_v3Min.x, m_v3Max.z - m_v3Min.z);

	drawMap();
}

std::vector<matrix4> MapReader::drawMap()
{
	//take the dimensions of the plane and divide it by the amount of squares desired 
	//note the the y does not mean y value. its a vector so it only has an x and a y.
	vector2 oneunit = vector2((dimensionofmodel.x / desireddimensions),(dimensionofmodel.y / desireddimensions));

	//makea list of mat4s to return. These mat4s will be the completed scale and transformation matrix
	std::vector<matrix4> walllist;

	int increment = 0;

	//going thru all the units 
	for (int i = 0; i < desireddimensions; i++) //represents rows
	{
		for (int j = 0; j < desireddimensions; j++) //represents columns
		{
			//one unit
			matrix4 currentunit = IDENTITY_M4;

			//translate it based on which cell we are in 
			currentunit *= glm::translate(vector3(i*oneunit.x, 0.0f, j*oneunit.y));

			//Translate it based on the text file if necessary
			if (filecontent[increment] == '-') // top wall
				currentunit *= glm::translate(vector3(oneunit.x, 0.0f, 0.0f));
			if (filecontent[increment] == '_')  //bottom wall
				currentunit *= glm::translate(vector3(-oneunit.x, 0.0f, 0.0f));
			if (filecontent[increment] == '|') //right wall
				currentunit *= glm::translate(vector3(0.0f, 0.0f, oneunit.y));

			//Scale it  based on the text file
			if (filecontent[increment] == '-') // smush to the top
				currentunit *= glm::scale();
			if (filecontent[increment] == '_')  //smush to the bottom
				currentunit *= glm::scale();
			if (filecontent[increment] == '|') //smush to the right
				currentunit *= glm::scale();

			increment++;

			//add it to the list
			walllist.push_back(currentunit);
		}
	}
	return walllist;
}



MapReader::~MapReader()
{
}
