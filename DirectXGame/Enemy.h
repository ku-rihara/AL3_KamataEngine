#pragma once

//state
#include"BaseEnemyState.h"
#include"EnemyStateApproach.h"
//
#include "Model.h"
#include "WorldTransform.h"
//class
#include"EnemyBullet.h"
#include"Collider.h"
#include"TimedCall.h"
//std
#include<list>

//前方宣言
class Player;

class Enemy :public Collider{
private:
	//包含
	EnemyBullet* enemyBullet_=nullptr;
	Player* player_ = nullptr;
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
	void ChangeState(std::unique_ptr<BaseEnemyState> state); // 状態変更

	// 衝突が検知されたら呼び出されるコールバック関数
	void OnColligion();
	//getter
	Vector3 GetWorldPos()override;
	std::list<TimedCall*> GetTimedCalls() const { return timedCalls_; }
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }
	//setter
	void SetPlayer(Player* player) { player_ = player; }
};
