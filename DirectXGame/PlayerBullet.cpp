#include "PlayerBullet.h"
#include "TextureManager.h"
#include"assert.h"
#include"CollisionConfig.h"


void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTarnsform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTarnsform_.translation_ = position;
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Update() {
	//座標を移動させる
	worldTarnsform_.translation_ += velocity_;
	if (--deathTimer_ <= 0) {
		isDeath_ = true;
	}
	worldTarnsform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTarnsform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTarnsform_.matWorld_.m[3][0];
	worldPos.y = worldTarnsform_.matWorld_.m[3][1];
	worldPos.z = worldTarnsform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnColligion() { isDeath_ = true; }