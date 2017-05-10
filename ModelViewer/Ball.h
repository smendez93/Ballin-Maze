#pragma once
#include "BallMaterial.h"
#include "RE\ReEngAppClass.h"
class Ball
{

public:
	std::string name;
	BallMaterial material;

	vector3 position;
	vector3 velocity;
	float scale;

	matrix4 m4Translation;
	matrix4 m4Rotation;
	matrix4 m4Scale;
	
	Ball(std::string name, vector2 position, BallMaterial material );

	matrix4 Update(matrix4 planeRotation);
	matrix4 GetMatrix();

	vector3 GetCenterGlobal();
	float GetRadius();

	~Ball();
};

