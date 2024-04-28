#include "Easing.h"
#include <cmath>

float Lerp(const float& start, const float& end, float t) {
	return (1.0f - t) * start + end * t;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	Vector3 result;
	result.x = (1.0f - t) * start.x + end.x * t;
	result.y = (1.0f - t) * start.y + end.y * t;
	result.z = (1.0f - t) * start.z + end.z * t;
	return result;
}

Vector3 SLerp(const Vector3& start, const Vector3& end, float t) {
	Vector3 Nstart = Normnalize(start);
	Vector3 Nend = Normnalize(end);
	// 内積を求める
	float dot = Dot(Nstart, Nend);
	// 誤差により1.0fを超えるのを防ぐ
	if (dot > 1.0f) {
		dot = 1.0f;
	}
	// アークコサインでθの角度を求める
	float theta = std::acos(dot);
	// θの角度からsinθを求める
	float sinTheta = std::sin(theta);
	// サイン(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	// サインθtを求める
	float sinThetaTo = std::sin(t * theta);
	Vector3 NormalizeVector;
	if (sinTheta < 1.0e-5) {
		NormalizeVector = Nstart;
	} else {
		// 球面線形補間したベクトル(単位ベクトル)
		NormalizeVector.x = (sinThetaFrom * Nstart.x + sinThetaTo * Nend.x) / sinTheta;
		NormalizeVector.y = (sinThetaFrom * Nstart.y + sinThetaTo * Nend.y) / sinTheta;
		NormalizeVector.z = (sinThetaFrom * Nstart.z + sinThetaTo * Nend.z) / sinTheta;
	}
	//ベクトルの長さはstartとendの長さを線形補間
	float length1 = Length(start);
	float length2 = Length(end);
	//Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);
	//長さを反映
	return NormalizeVector*length;
}