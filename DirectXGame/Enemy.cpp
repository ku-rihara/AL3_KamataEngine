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
	//メンバ関数ポインタに入ってる関数を呼び出す
	(this->*phaseTable[static_cast<size_t>(phase_)])();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
void Enemy::Approach() {
	// 移動ベクトルを加算
	worldTransform_.translation_ -= velocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave() {
	//移動ベクトルを加算
	Vector3 leaveV(-0.3f, 0.3f, 0);
	leaveV = TransformNormal(leaveV, worldTransform_.matWorld_);
	worldTransform_.translation_ += leaveV;
}
//メンバ関数ポインタテーブル
void (Enemy::*Enemy::phaseTable[])() = {
    &Enemy::Approach,//接近
    &Enemy::Leave,//離脱
};