#pragma once
#include"Model.h"
#include"WorldTransform.h"
class Enemy {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_=0;
	Vector3 velocity_;

public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
};
