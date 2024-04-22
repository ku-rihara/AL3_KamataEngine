#include "Enemy.h"
#include"TextureManager.h"
#include"assert.h"
#include"Geometry/fMatrix4x4.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {

}

void Enemy::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("sample.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void Enemy::Update() { 
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);
	worldTransform_.translation_ -= velocity_;
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}