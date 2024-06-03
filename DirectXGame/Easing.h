#pragma once
#include"Vector3.h"
#include"Vector2.h"

float Lerp(const float& start, const float& end, float t);

Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

Vector3 SLerp(const Vector3& start, const Vector3& end, float t);