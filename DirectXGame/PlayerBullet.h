#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include"Collider.h"
/// <summary>
/// 自キャラの弾
/// </summary>
class GameScene;
class Enemy;
class PlayerBullet:public Collider {
private:
	// 包含
	GameScene* gameScene_ = nullptr;


	static const int32_t kLifeTime = 60 * 5;//寿命
	int32_t deathTimer_ = kLifeTime;//デスタイマー
	bool isDeath_ = false; // デスフラグ
	bool isHoming_;
	
	Model* model_;
	WorldTransform worldTarnsform_;
	Vector3 startPos_;
	Vector3 velocity_; // 速度
	uint32_t textureHandle_ = 0;
	Enemy* targetEnemy_;

public:
	
	void Init(Model* model, const Vector3& position, const Vector3& velocity, bool isHorming);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void OnColligion();
	void Directionoftravel();


	Vector3 GetWorldPos() override;
	bool GetIsDead() const { return isDeath_; }
	void SetGameScene(GameScene* scene) { gameScene_ = scene; }

	
};
