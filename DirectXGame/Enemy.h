#pragma once
#include"Model.h"
#include"WorldTransform.h"
// 行動フェーズ
enum class Phase {
	Initial,
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_=0;
	Vector3 velocity_;
	
	// フェーズ
	Phase phase_=Phase::Initial;
public:
	Enemy();
	~Enemy();
	void Init(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void ApproachUpdate();
	void LeaveUpdate();
};
