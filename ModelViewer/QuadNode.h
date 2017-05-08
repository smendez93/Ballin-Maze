#pragma once
#include "RE\ReEngAppClass.h"
#include "MyEntityClass.h"


class QuadNode
{
private:
	vector3 min;
	vector3 max;

	QuadNode(vector3 min, vector3 max, unsigned int depth);
	void GenerateChildren(unsigned int nextDepth);
	void CalculateHull(std::vector<MyEntityClass*> objects);
	void AddAll(std::vector<MyEntityClass*> objects);
	bool Add(MyEntityClass* object);
	bool Contains(MyBoundingBoxClass* box);
	bool Collides(MyBoundingBoxClass* box);
public:

	//+x = 0_2_
	//+z = 01__
	QuadNode* nodes[4]; // Child nodes

	std::vector<MyEntityClass*> objs;
	QuadNode(std::vector<MyEntityClass*> objects, unsigned int depth);

	~QuadNode();

	bool CheckCollision(MyEntityClass* object);
	void Render();
};

