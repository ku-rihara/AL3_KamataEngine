#pragma once
#include "Model.h"
#include "WorldTransform.h"
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	// フェーズ
	Phase phase_;

public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void Leave();

private:
	// メンバ関数のポインタのテーブル
	static void (Enemy::*phaseTable[])();
};
