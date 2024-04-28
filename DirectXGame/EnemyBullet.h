#pragma once
#include"WorldTransform.h"
#include"Model.h"

//前方宣言
class Player;

class EnemyBullet {
private:
	Model* model_ = nullptr;
	Player* player_;

	static const int32_t kLifeTime = 60 * 5;//寿命
	int32_t deathTimer_ = kLifeTime;//デスタイマー
	WorldTransform worldTransform_;
	Vector3 velocity_;
	uint32_t TextureHandle_;
	bool isDeath_ = false; // デスフラグ
public:
	EnemyBullet();
	~EnemyBullet();
	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection&viewProjection);

	void Directionoftravel();
	//getter
	Vector3 GetWorldPos();
	bool GetIsDeath() const { return isDeath_; }
	//setter
	void SetPlayer(Player* player) { player_ = player; }
};
