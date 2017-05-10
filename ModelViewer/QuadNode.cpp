#include "QuadNode.h"

QuadNode::QuadNode(std::vector<Wall*> objects, unsigned int depth)
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

void QuadNode::CalculateHull(std::vector<Wall*> objects)
{
	// Calc min/max values
	vector3 hullMin = vector3(FLT_MAX);
	vector3 hullMax = vector3(-FLT_MAX);
	for (Wall* obj : objects) {
		vector3 obMin = obj->GetCenterGlobal() - obj->GetHalfWidth();
		vector3 obMax = obj->GetCenterGlobal() + obj->GetHalfWidth();

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

void QuadNode::AddAll(std::vector<Wall*> objects)
{
	// For each obj in parent...
	for (Wall* obj : objects) {
		Add(obj);
	}
}

bool QuadNode::Add(Wall * object)
{
	// If object is not contained in the child
	if (!Contains(object))
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

bool QuadNode::Contains(Wall* box)
{
	// Check if box is within the node
	vector3 b_min = box->GetCenterGlobal() - box->GetHalfWidth();
	vector3 b_max = box->GetCenterGlobal() + box->GetHalfWidth();
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

bool QuadNode::Collides(Ball * ball)
{
	// Check box collisions

	vector3 b_min = ball->GetCenterGlobal() - vector3(ball->GetRadius());
	vector3 b_max = ball->GetCenterGlobal() + vector3(ball->GetRadius());
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

bool QuadNode::IsColliding(Ball * ball, Wall * wall, vector3 & normal)
{
	bool onEdge = false;
	vector3 point = ball->GetCenterGlobal() - wall->GetCenterGlobal();
	point.y = 0;
	vector3 bounds = wall->GetHalfWidth();
	bounds.y = 0;

	//bind point to wall collision box
	if (point.x > bounds.x)
	{
		point.x = bounds.x;
		onEdge = true;
	}
	if (point.z > bounds.z)
	{
		point.z = bounds.z;
		onEdge = true;
	}

	if (point.x < -bounds.x)
	{
		point.x = -bounds.x;
		onEdge = true;
	}
	if (point.z < -bounds.z)
	{
		point.z = -bounds.z;
		onEdge = true;
	}

	if (!onEdge) {
		vector3 slope = point / wall->GetHalfWidth();
		if (slope.x < 0) slope.x = -slope.x;
		if (slope.z < 0) slope.z = -slope.z;

		if (slope.x > slope.z)
		{
			//tertiary to maintain sign
			point.x = wall->GetHalfWidth().x*(point.x > 0 ? 1 : -1);
		}
		else
		{
			//tertiary to maintain sign
			point.z = wall->GetHalfWidth().z*(point.z > 0 ? 1 : -1);
		}
	}
	//find vector from closest wall point to ball
	vector3 distance = (ball->GetCenterGlobal() - wall->GetCenterGlobal()) - point;
	distance.y = 0;

	if (!onEdge || glm::length(distance) < ball->GetRadius())
	{
		vector3 rePosition = vector3(0.f);
		// collision
		if (onEdge)
		{
			rePosition = (distance*ball->GetRadius() / glm::length(distance))-distance;
			normal = (distance / glm::length(distance));
		}
		else {
			rePosition = -(distance*ball->GetRadius() / glm::length(distance)) - distance;
			normal = -(distance / glm::length(distance));
		}
		rePosition.y = 0;
		normal.y = 0;
		ball->position += rePosition;
		ball->m4Translation = glm::translate(ball->position);
		return true;
	}
	normal = vector3(0.f);
	return false;
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


bool QuadNode::CheckCollision(Ball * object, vector3& reflect)
{
	vector3 tempReflect = vector3(0.f);
	reflect = tempReflect;
	if (!Collides(object))
	{
		return false;
	}
	bool anyCollide = false;
	// For each obj
	for (unsigned int a = 0; a < objs.size(); a++)
	{
		if (IsColliding(object, objs[a], tempReflect))
		{
			reflect += tempReflect;
			anyCollide = true;
		}
	}

	// Check child nodes
	for (unsigned int n = 0; n < 4; n++)
	{
		if (nodes[n] != nullptr && nodes[n]->CheckCollision(object, tempReflect))
		{
			reflect += tempReflect;
			anyCollide = true;
		}
	}
	return anyCollide;
}


QuadNode::~QuadNode()
{
	if (nodes[0] != nullptr) {
		for (unsigned int i = 0; i < 4; i++) {
			delete nodes[i];
		}
	}
}
