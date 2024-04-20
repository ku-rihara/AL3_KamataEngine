#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"input/input.h"
//func
#include"Geometry/fMatrix4x4.h"
//class
#include"PlayerBullet.h"
//std
#include<list>


class Player {
private:
	//包含
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	//弾
	std::list<PlayerBullet*> bullets_;
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
