#include "EnemyBullet.h"
#include"assert.h"
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
	//Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x,velocity_.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4x4 matrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	//velocity_に回転行列を掛け算してvelocityZを求める
	Vector3 velocityZ = Multiply(velocity_, matrix);
	//X軸周り角度(θX)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	TextureHandle_ = TextureManager::Load("uvChecker.png");
}

void EnemyBullet::Update() {
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