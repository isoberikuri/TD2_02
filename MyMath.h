#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
using namespace MathUtility;


KamataEngine::Matrix4x4 MakeAffineMatrix(Vector3& scale, Vector3& rotation, Vector3& translation);
// イージング
float EaseInOut(float x1, float x2, float t);

struct AABB {
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2);
