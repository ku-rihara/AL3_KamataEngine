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
// std
#include <list>
class GameScene;
//class Enemy;
class Player:public Collider {
private:
	// 包含
	/*Enemy* enemy_ = nullptr;*/
	GameScene* gameScene_;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	Model* model_ = nullptr;
	Sprite* sprite2DReticle_ = nullptr;
	Vector2 Reticle2DPos_;
	Vector2 rockPos_;
	bool isRockOn_;
	float reticleMoveTime_;
	WorldTransform worldTransform_; // ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	
	// 弾
	std::list<PlayerBullet*> bullets_;
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
	void Update(const ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);
	void DrawUI();
	void RockOn();
	void CanselRockOn();
	//衝突が検知されたら呼び出されるコールバック関数
	void OnColligion()override;

	//getter
	Vector3 GetWorldPos()override;
	Vector3 GetWorld3DRecticlPos();
	Vector2 Getsprite2DreticlePos() const { return sprite2DReticle_->GetPosition(); }
	Vector2 Get2DreticlePos() const { return Reticle2DPos_; }
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	//setter
	void SetParent(const WorldTransform* parent);
	void SetGameScene(GameScene* scene) { gameScene_ = scene; }
	
};
