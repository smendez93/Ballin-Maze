#pragma once

struct BallMaterial {
	float friction;
	float drag;
	float bounce;
	float gravity;
};
namespace BallMat {
	const BallMaterial normal = { 0.f,0.f,0.5f,1.f };
}