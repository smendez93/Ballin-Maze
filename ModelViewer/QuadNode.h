#pragma once
#include "RE\ReEngAppClass.h"
#include "Wall.h"
#include "Ball.h"

class QuadNode
{
private:
	vector3 min;
	vector3 max;

	QuadNode(vector3 min, vector3 max, unsigned int depth);
	void GenerateChildren(unsigned int nextDepth);
	void CalculateHull(std::vector<Wall*> objects);
	void AddAll(std::vector<Wall*> objects);
	bool Add(Wall* object);
	bool Contains(Wall* box);
	bool Collides(Ball* box);

	bool IsColliding(Ball* ball, Wall* wall, vector3& normal);

public:

	//+x = 0_2_
	//+z = 01__
	QuadNode* nodes[4]; // Child nodes

	std::vector<Wall*> objs;
	QuadNode(std::vector<Wall*> objects, unsigned int depth);

	~QuadNode();

	bool CheckCollision(Ball* object, vector3& reflect);
	void Render();
};

