#pragma once
#include <memory>
#include "Model.h"
#include "WorldTransform.h"
#include "input/input.h"

class Player {
private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	 Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	

	void Init(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
};
