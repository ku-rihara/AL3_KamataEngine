#pragma once
#include"WorldTransform.h"
#include"Model.h"

class EnemyBullet {
private:
	static const int32_t kLifeTime = 60 * 5;//寿命
	int32_t deathTimer_ = kLifeTime;//デスタイマー
	bool isDeath_ = false;//デスフラグ
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	uint32_t TextureHandle_;

public:
	EnemyBullet();
	~EnemyBullet();
	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection&viewProjection);

	// 衝突が検知されたら呼び出されるコールバック関数
	void OnColligion();
	Vector3 GetWorldPos();
	bool GetIsDeath() const { return isDeath_; }
};
