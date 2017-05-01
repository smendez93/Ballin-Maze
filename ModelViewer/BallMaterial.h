#pragma once

struct BallMaterial {
	float friction;
	float drag;
	float bounce;
	float gravity;
};
namespace BallMat {
	const BallMaterial normal = { 0.f,0.02f,0.5f,0.4f };
}