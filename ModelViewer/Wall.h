#pragma once
#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
class Wall
{
	enum wallType {
		none = 0,
		down = 1,
		right = 2
	};
public:
	Wall(vector2 pos, int t, int ind);
	Wall();
	~Wall();

	vector3 position;
	wallType type;
	matrix4 m4Transform;
	std::string name;
	
	const vector3 scale = vector3(1.f, 1.f, 0.2f);
	const float SIZE = 0.8f;
};

