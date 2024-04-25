#pragma once

//state
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"
//
#include "Model.h"
#include "WorldTransform.h"


class Enemy {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

//状態

	std::unique_ptr<BaseEnemyState> state_;
public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Move(Vector3 velocity);
	//状態変更
	void ChangeState(std::unique_ptr<BaseEnemyState> state);
	//getter
	Vector3 GetWorldTransform() const { return worldTransform_.translation_; }
};
