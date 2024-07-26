#pragma once
#include"Colider.h"
#include "Model.h"
#include"WorldTransform.h"
class Player;
class Hummer final :public Colider {
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

public:
	void SetParent(const WorldTransform& worldTransform);
	void Init(const Model& models);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
};
