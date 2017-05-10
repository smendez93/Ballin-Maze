#pragma once

#define GRAV 0.7f

struct BallMaterial {
	float drag;
	float bounce;
	float gravity;
};
namespace BallMat {
	const BallMaterial rubber = { 0.001f,0.98f,GRAV };
	const BallMaterial fuzzy = { 0.04f,0.2f,GRAV/2 };
	const BallMaterial lead = { 0.01f,0.1f,GRAV*2 };
}