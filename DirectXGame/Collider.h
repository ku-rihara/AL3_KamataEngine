#pragma once
#include"Vector3.h"
class Collider {
private:
	// 衝突半径
	float radius_ = 5;

 public:
	virtual void OnColligion();
	// getter
	virtual Vector3 GetWorldPos() = 0;
	float GetRadius() const { return radius_; }
	// setter
	void SetRadius(float radius) { this->radius_ = radius; }
};
