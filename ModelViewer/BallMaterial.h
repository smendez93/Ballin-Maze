#pragma once

#define GRAV 0.7f
//Forces on the ball
struct BallMaterial {
	float drag;
	float bounce;
	float gravity;
};

//The different materials
namespace BallMat {
	const BallMaterial rubber = { 0.001f,0.98f,GRAV }; // crazy bounce coefficient 
	const BallMaterial fuzzy = { 0.04f,0.2f,GRAV/2 }; // much more drag
	const BallMaterial lead = { 0.01f,0.1f,GRAV*2 }; // less bounce, less drag but higher gravity
}