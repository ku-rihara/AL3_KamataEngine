#pragma once
#include "WorldTransform.h"
#include"Vector3.h"
class Collider {
private:
	// 衝突半径
	float radius_ = 5;
	//衝突属性（自分）n
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク（相手）
	uint32_t collisionMask_ = 0xffffffff;

 public:
	virtual void OnColligion();
	// getter
	virtual Vector3 GetWorldPos() = 0;
	float GetRadius() const { return radius_; }
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	uint32_t GetCollisionMask() const { return collisionMask_; }
	// setter
	void SetRadius(float radius) { this->radius_ = radius; }
	void SetCollisionAttribute(uint32_t bit) { this->collisionAttribute_ = bit; }
	void SetCollisionMask(uint32_t bit) { this->collisionMask_ = bit; }
};
