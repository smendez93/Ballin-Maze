#include "Ball.h"


Ball::Ball(std::string name, vector2 position, BallMaterial material = BallMat::normal)
{
	this->name = name;
	this->position = vector3(position.x,.2f,position.y);
	this->velocity = vector3(0.f);
	this->material = material;
	this->m4Scale = glm::scale(vector3(.2f));
	this->m4Translation = glm::translate(this->position);
	this->m4Rotation = IDENTITY_M4;
}

matrix4 Ball::Update(matrix4 planeRotation)
{
	vector4 grav(glm::normalize(-REAXISY)*material.gravity, 1);
	grav = planeRotation*grav;
	velocity += vector3(grav.x*.01f, 0, grav.z*.01f);
	velocity -= velocity*material.drag;
	quaternion start = quaternion(vector3(0.f, 0.f, 0.f));
	quaternion end = quaternion(vector3(glm::dot(REAXISZ,velocity), 0.f, glm::dot(-REAXISX, velocity)));
	quaternion roll = glm::mix(start, end, (float)(velocity.length()));

	m4Translation *= glm::translate(velocity);
	m4Rotation = glm::mat4_cast(roll)*m4Rotation;
	return GetMatrix();
}

matrix4 Ball::GetMatrix()
{
	return m4Translation * m4Rotation * m4Scale;
}

Ball::~Ball()
{
}
