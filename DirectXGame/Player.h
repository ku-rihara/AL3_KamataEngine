#pragma once
#include "Model.h"
#include"Sprite.h"
#include "WorldTransform.h"
#include "input/input.h"
// func
#include "Geometry/fMatrix4x4.h"
// class
#include "PlayerBullet.h"
#include"Collider.h"
#include"Reticle2D.h"
// std
#include <list>
class GameScene;
class Reticle2D;
//class Enemy;
class Player:public Collider {
private:
	// 包含
	Reticle2D* reticle2D_; 
	GameScene* gameScene_;
	/*Enemy* enemy_;*/

	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	Model* model_ = nullptr;
	
	bool isRockOn_;
	WorldTransform worldTransform_; // ワールドトランスフォーム
	
	// 弾
	std::list<PlayerBullet*> bullets_;
	/*std::list<Enemy*> enemys_;*/

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	void Rotate(); // 回転
	void Move();   // 移動
	void Attack(); // 攻撃
public:
	Player();
	~Player();

	void Init(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void RockOn();
	void CanselRockOn();
	//衝突が検知されたら呼び出されるコールバック関数
	void OnColligion()override;
	void ReticleInit();

	//getter
	Vector3 GetWorldPos()override;
	Matrix4x4 GetMatWorld() const { return worldTransform_.matWorld_; }
	bool GetIsRockOn() const { return isRockOn_; }
	
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	//setter
	void SetParent(const WorldTransform* parent);
	void SetGameScene(GameScene* scene) { gameScene_ = scene; }
	/*void SetEnemy(Enemy* e) { enemy_ = e; }*/
	void SetReticle2D(Reticle2D* reticle) { reticle2D_ = reticle; }

	/*void SetEnemys(Enemy* enemys) {  enemys_.push_back(enemys); }
	*/
};
