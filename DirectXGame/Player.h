#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"input/input.h"
#include"Geometry/fMatrix4x4.h"



class Player {
private:
	//包含
	Input* input_ = nullptr;
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
};
