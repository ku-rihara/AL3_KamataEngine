#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"input/input.h"
#include"Geometry/fMatrix4x4.h"
#include"PlayerBullet.h"



class Player {
private:
	//包含
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
public:

	Player();
	~Player();

	void Init(Model*model,uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection&viewProjection);

private:
	void Rotate(); // 回転
	void Move();//移動
	void Attack(); // 攻撃
};
