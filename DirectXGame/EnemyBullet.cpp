#include "EnemyBullet.h"
#include"assert.h"
#include"TextureManager.h"
#include"CollisionConfig.h"

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
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
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

Vector3 EnemyBullet::GetWorldPos() {
	
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnColligion() { isDeath_ = true; }