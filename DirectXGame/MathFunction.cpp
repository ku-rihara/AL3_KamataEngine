#include "MathFunction.h"
#include <assert.h>
#include <cmath>

float Clamp(float n, float min, float max) {
	if (n > max) {
		return max;
	}
	if (n < min) {
		return min;
	}
	return n;
}

size_t Clamp(size_t n, size_t min, size_t max) {
	if (n > max) {
		return max;
	}
	if (n < min) {
		return min;
	}
	return n;
}

Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3 p2, const Vector3& p3, float t) {
	Vector3 result;
	const float s = 0.5f;
	float t2 = t * t;
	float t3 = t2 * t;
	Vector3 e3;
	Vector3 e2;
	Vector3 e1;
	Vector3 e0;
	e3.x = -p0.x + 3 * p1.x - 3 * p2.x + p3.x;
	e3.y = -p0.y + 3 * p1.y - 3 * p2.y + p3.y;
	e3.z = -p0.z + 3 * p1.z - 3 * p2.z + p3.z;
	e2.x = 2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x;
	e2.y = 2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y;
	e2.z = 2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z;
	e1.x = -p0.x + p2.x;
	e1.y = -p0.y + p2.y;
	e1.z = -p0.z + p2.z;
	e0.x = 2 * p1.x;
	e0.y = 2 * p1.y;
	e0.z = 2 * p1.z;
	result.x = s * (e3.x * t3 + e2.x * t2 + e1.x * t + e0.x);
	result.y = s * (e3.y * t3 + e2.y * t2 + e1.y * t + e0.y);
	result.z = s * (e3.z * t3 + e2.z * t2 + e1.z * t + e0.z);
	return result;
}

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は４点以上必要です");
	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0fとした割合)
	float areaWidth = 1.0f / division;
	// 区間内の始点0.0f、終点を1.0fとした時の現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)とした時の現在位置
	t_2 = Clamp(t_2, 0.0f, 1.0f);
	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = Clamp(index, 0, division - 1);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;
	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}
	// 最初の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	// 4点を指定してCamull-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}