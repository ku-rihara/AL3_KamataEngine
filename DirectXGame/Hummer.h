#pragma once
#include"Colider.h"
#include "Model.h"
#include"WorldTransform.h"


class Hummer final :public Colider {
private:
	
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	float effectEase = 0;
	
public:
	Hummer();
	void SetParent(const WorldTransform& worldTransform);
	void Init( Model* models);
	void Update();
	void Attack(float easeT);
	void Draw(const ViewProjection& viewProjection);
	void HitEffectUpdate();
	void HitEffectInit();

	Vector3 GetBaseCenterPosition() const override;
	 void OnCollision([[maybe_unused]] Colider* other) override;
	//getter
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
	
	
	
};
