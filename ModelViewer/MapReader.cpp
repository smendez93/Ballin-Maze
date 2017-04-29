#include "MapReader.h"

MapReader::MapReader(std::vector<vector3> a_lVectorList)
{
}
std::vector<Wall*> MapReader::ParseFile(std::string file)
{			
	String filecontent;
	char type;
	std::vector<Wall*> walls;
	FileReaderClass reader;
	reader.ReadFile(file);
	int ind = 0;
			
	for (int r = 0; r < DIMS; r++)
	{
		//reading each line and saving it to filecontent
		reader.ReadNextLine(false);	
		filecontent = reader.m_sLine;
		for (int c = 0; c < DIMS; c++)
		{
			type = filecontent[c];
			switch (type)
			{
			case '.':
				break;
			case '_':
				walls.push_back(new Wall(vector2(c - 6, r - 6), 1, ind));
				ind++;
				break;
			case '|':
				walls.push_back(new Wall(vector2(c - 6, r - 6), 2, ind));
				ind++;
				break;
			case 'L':
				walls.push_back(new Wall(vector2(c - 6, r - 6), 1, ind));
				ind++;
				walls.push_back(new Wall(vector2(c - 6, r - 6), 2, ind));
				ind++;
				break;
			}
		}
	}
	reader.CloseFile();
	return walls;
}

MapReader::~MapReader()
{
}
