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
	switch (phase_) {
	case Phase::Approach:
	default:
		//移動（ベクトルを加算）
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	/*worldTransform_.translation_ -= velocity_;*/
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
void Enemy::ApproachUpdate() {
	// 移動ベクトルを加算
	worldTransform_.translation_ -= velocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::LeaveUpdate() {
	//移動ベクトルを加算
	Vector3 leaveV(-0.3f, 0.3f, 0);
	leaveV = TransformNormal(leaveV, worldTransform_.matWorld_);
	worldTransform_.translation_ += leaveV;
}