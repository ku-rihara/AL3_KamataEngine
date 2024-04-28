#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "input/input.h"
// func
#include "Geometry/fMatrix4x4.h"
// class
#include "PlayerBullet.h"
// std
#include <list>

class Player {
private:
	// 包含
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	Model* model_ = nullptr;

	WorldTransform worldTransform_; // ワールドトランスフォーム
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
	void Update();
	void Draw(ViewProjection& viewProjection);

	//衝突が検知されたら呼び出されるコールバック関数
	void OnColligion();

	//getter
	Vector3 GetWorldPos();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
};
