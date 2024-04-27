#pragma once

//state
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
//
#include "Model.h"
#include "WorldTransform.h"
//class
#include"EnemyBullet.h"
#include"TimedCall.h"
//std
#include<list>

class Enemy {
private:
	//包含
	EnemyBullet* enemyBullet_=nullptr;
	Model* model_=nullptr;

	
	WorldTransform worldTransform_;
	Vector3 velocity_;
	uint32_t textureHandle_ = 0;
	
	std::list<EnemyBullet*> enemyBullets_;  // 弾
	std::list<TimedCall*> timedCalls_;//時限発動のリスト
    std::unique_ptr<BaseEnemyState> state_;//状態

public:
	//発射間隔
	static const int kFireInterval = 60;
public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ApproachInit();//初期フェーズ
	void Move(Vector3 velocity);//移動
	void Fire();//弾の発射
	void FireAndReset();

	void ChangeState(std::unique_ptr<BaseEnemyState> state);//状態変更
	//getter
	Vector3 GetWorldTransform() const { return worldTransform_.translation_; }
};
