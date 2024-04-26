#include "EnemyBullet.h"
#include"assert.h"
#include"TextureManager.h"

EnemyBullet::EnemyBullet() {

}

EnemyBullet::~EnemyBullet() {

}

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
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