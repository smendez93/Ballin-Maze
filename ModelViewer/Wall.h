#pragma once
#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
class Wall
{
public:
	enum wallType {
		none = 0,
		down = 1,
		right = 2
	};
	Wall(vector2 pos, int t, int ind);
	Wall();
	~Wall();

	vector3 position;
	vector3 m_v3Bounds;
	wallType type;
	matrix4 m4Transform;
	std::string name;

	const vector3 scale = vector3(1.f, 1.f, 0.25f);
	const float SIZE = 0.8f;


	vector3 GetCenterGlobal();
	vector3 GetHalfWidth();

};

