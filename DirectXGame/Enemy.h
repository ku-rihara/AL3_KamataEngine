#pragma once

//state
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
//
#include "Model.h"
#include "WorldTransform.h"
//class
#include"EnemyBullet.h"
//std
#include<list>

class Enemy {
private:
	//包含
	EnemyBullet* enemyBullet_=nullptr;
	Model* model_=nullptr;
	// 弾
	std::list<EnemyBullet*> enemyBullets_;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	int32_t fireTimer_;
	uint32_t textureHandle_ = 0;
	//状態
    std::unique_ptr<BaseEnemyState> state_;

public:
	//発射間隔
	static const int kFireInterval = 60;


public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ApproachInit();
	void Move(Vector3 velocity);
	void Fire();

	void ChangeState(std::unique_ptr<BaseEnemyState> state);//状態変更
	//getter
	Vector3 GetWorldTransform() const { return worldTransform_.translation_; }
	uint32_t GetFireTimer() const { return fireTimer_; }

	//
	void SetFireTimer(uint32_t time) { this->fireTimer_ = time; }
	void FireTimerDecrement() { this->fireTimer_--; }
};
