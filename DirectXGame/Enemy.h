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
class GameScene;

class Enemy :public Collider{
private:
	
	//包含
	GameScene* gameScene_ = nullptr;
	EnemyBullet* enemyBullet_=nullptr;
	Player* player_ = nullptr;
	Model* model_=nullptr;
	bool isdeath_=false;
	bool isTarget_;
	
	
	WorldTransform worldTransform_;
	Vector2 ScreenPos_;
	Vector3 velocity_;
	uint32_t textureHandle_ = 0;
	
	
	std::list<TimedCall*> timedCalls_;//時限発動のリスト
    std::unique_ptr<BaseEnemyState> state_;//状態

public:
	//発射間隔
	static const int kFireInterval = 60;
public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update(const ViewProjection& viewProjection);
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
	Vector2 GetScreenPos() const { return ScreenPos_; }
	bool GetIsDeath() const { return isdeath_; }
	bool GetIsTarget() const { return isTarget_; }
	std::list<TimedCall*> GetTimedCalls() const { return timedCalls_; }
		//setter
	void SetIsTarget(bool is) { isTarget_ = is; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void SetPlayer(Player* player) { player_ = player; }
};
