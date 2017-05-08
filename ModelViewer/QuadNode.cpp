#include "QuadNode.h"

QuadNode::QuadNode(std::vector<MyEntityClass*> objects, unsigned int depth)
{
	// Calc min/max
	CalculateHull(objects);

	// Init noes
	for (unsigned int i = 0; i < 4; i++)
	{
		nodes[i] = nullptr;
	}
	// Generate children nodes
	if (depth > 0)
	{
		GenerateChildren(depth - 1);
	}
	// Fill nodes
	AddAll(objects);
}

QuadNode::QuadNode(vector3 min, vector3 max, unsigned int depth)
{
	// Set min/max
	this->min = min;
	this->max = max;
	// Init nodes
	for (unsigned int i = 0; i < 4; i++)
	{
		nodes[i] = nullptr;
	}
	// Generate child nodes
	if (depth > 0)
	{
		GenerateChildren(depth - 1);
	}
}

void QuadNode::GenerateChildren(unsigned int nextDepth)
{
	// Calc
	vector3 center = (min + max) / 2.f;
	vector3 size = center - min;

	//+x = 0_2_
	//+z = 01__

	for (unsigned int i = 0; i < 4; i++) {
		vector3 localMin = vector3(
			(i & 1) ? min.x : center.x,
			0,
			(i & 2) ? min.z : center.z);
		vector3 localMax = localMin + size;
		nodes[i] = new QuadNode(localMin, localMax, nextDepth);
	}
}

void QuadNode::CalculateHull(std::vector<MyEntityClass*> objects)
{
	// Calc min/max values
	vector3 hullMin = vector3(FLT_MAX);
	vector3 hullMax = vector3(-FLT_MAX);
	for (MyEntityClass* obj : objects) {
		MyBoundingBoxClass* box = obj->GetBoundingBox();
		vector3 obMin = box->GetCenterGlobal() - box->GetHalfWidthG();
		vector3 obMax = box->GetCenterGlobal() + box->GetHalfWidthG();

		if (hullMin.x > obMin.x) hullMin.x = obMin.x;
		if (hullMin.y > obMin.y) hullMin.y = obMin.y;
		if (hullMin.z > obMin.z) hullMin.z = obMin.z;

		if (hullMax.x < obMax.x) hullMax.x = obMax.x;
		if (hullMax.y < obMax.y) hullMax.y = obMax.y;
		if (hullMax.z < obMax.z) hullMax.z = obMax.z;
	}
	this->min = hullMin;
	this->max = hullMax;
}

void QuadNode::AddAll(std::vector<MyEntityClass*> objects)
{
	// For each obj in parent...
	for (MyEntityClass* obj : objects) {
		Add(obj);
	}
}

bool QuadNode::Add(MyEntityClass * object)
{
	// If object is not contained in the child
	if (!Contains(object->GetBoundingBox()))
		return false;
	for (int i = 0; i < 4; i++)
	{
		// If the node is null
		if (nodes[i] == nullptr) continue;
		if (nodes[i]->Add(object)) // Can the obj be added
			return true;
	}
	// Add the obj
	objs.push_back(object);
	return true;
}

bool QuadNode::Contains(MyBoundingBoxClass* box)
{
	// Check if box is within the node
	vector3 b_min = box->GetCenterGlobal() - box->GetHalfWidthG();
	vector3 b_max = box->GetCenterGlobal() + box->GetHalfWidthG();
	if (   (b_min.x >= min.x)
		&& (b_min.z >= min.z)
		&& (b_max.x <= max.x)
		&& (b_max.z <= max.z)
		)
	{
		return true;
	}
	return false;
}

bool QuadNode::Collides(MyBoundingBoxClass * box)
{
	// Check box collisions

	vector3 b_min = box->GetCenterGlobal() - box->GetHalfWidthG();
	vector3 b_max = box->GetCenterGlobal() + box->GetHalfWidthG();
	bool bColliding = true;

	//Check for X
	if (max.x < b_min.x)
		bColliding = false;
	if (min.x > b_max.x)
		bColliding = false;

	//Check for Z
	if (max.z < b_min.z)
		bColliding = false;
	if (min.z > b_max.z)
		bColliding = false;

	return bColliding;
}

void QuadNode::Render()
{
	// Render cube wireframe
	matrix4 cubeSpot = glm::translate(((max+min)/2.f))*glm::scale(((max-min)));
	MeshManagerSingleton::GetInstance()->AddCubeToRenderList(cubeSpot, REYELLOW, WIRE);
	for (unsigned int n = 0; n < 4; n++)
	{
		if (nodes[n] != nullptr)
			nodes[n]->Render();

	}
}


bool QuadNode::CheckCollision(MyEntityClass * object)
{
	if (!Collides(object->GetBoundingBox()))
		return false;

	// For each obj
	for (unsigned int a = 0; a < objs.size(); a++)
	{
		if (objs[a] != object && object->IsColliding(objs[a]))
			return true;
	}

	// Check child nodes
	for (unsigned int n = 0; n < 4; n++)
	{
		if (nodes[n] != nullptr && nodes[n]->CheckCollision(object))
			return true;
		
	}
	return false;
}


QuadNode::~QuadNode()
{
	if (nodes[0] != nullptr) {
		for (unsigned int i = 0; i < 4; i++) {
			delete nodes[i];
		}
	}
}
