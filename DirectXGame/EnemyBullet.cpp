#include "EnemyBullet.h"
#include"Player.h"
#include"assert.h"
#include"Easing.h"
#include"TextureManager.h"
#include"Geometry/fMatrix4x4.h"
#include<cmath>

EnemyBullet::EnemyBullet() {

}

EnemyBullet::~EnemyBullet() {

}

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};
	velocity_ = velocity;
	
	Directionoftravel(); 
	TextureHandle_ = TextureManager::Load("uvChecker.png");
}

void EnemyBullet::Update() {
	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPos() - GetWorldPos();
	//ベクトルを正規化
	 toPlayer = Normnalize(toPlayer);
	 velocity_ = Normnalize(velocity_);
	//球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
	 velocity_ = SLerp( velocity_, toPlayer, 0.1f) * 1.0f;
	Directionoftravel(); 
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	if (--deathTimer_ <= 0) {
		isDeath_ = true;
	}
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, TextureHandle_);
}

void EnemyBullet::Directionoftravel() {
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4x4 matrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	// velocity_に回転行列を掛け算してvelocityZを求める
	Vector3 velocityZ = Multiply(velocity_, matrix);
	// X軸周り角度(θX)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
}

Vector3 EnemyBullet::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}