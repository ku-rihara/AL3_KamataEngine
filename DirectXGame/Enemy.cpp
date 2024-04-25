#include "Enemy.h"
#include "Geometry/fMatrix4x4.h"
#include "TextureManager.h"
#include "assert.h"


Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("sample.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
	ChangeState(std::make_unique<EnemyStateApproach>(this));
}

void Enemy::Update() {
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);
	// メンバ関数ポインタに入ってる関数を呼び出す
	state_->Update();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


void Enemy::Move(Vector3 velocity) { 
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ += velocity;
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { 
	state_ = std::move(state);
}