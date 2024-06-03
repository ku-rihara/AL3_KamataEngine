#include "PlayerBullet.h"
#include "CollisionConfig.h"
#include "TextureManager.h"
#include "assert.h"
#include"Geometry/fMatrix4x4.h"
#include<cmath>
//function
#include"Easing.h"
//class
#include "Enemy.h"

void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTarnsform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTarnsform_.translation_ = position;
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Update() {
	if (enemy_->GetIsTarget()) { //	ターゲットしてる
		                         
		Vector3 toEnemy = enemy_->GetWorldPos() - GetWorldPos();//プレイヤーから敵
		// ベクトルを正規化
		toEnemy = Normnalize(toEnemy);
		velocity_ = Normnalize(velocity_);
		// 球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
		velocity_ = SLerp(velocity_, toEnemy, 0.1f) * 1.0f;
		Directionoftravel();

	} else {

		// 座標を移動させる
		worldTarnsform_.translation_ += velocity_;
	}
	if (--deathTimer_ <= 0) {
		isDeath_ = true;
	}
	worldTarnsform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
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

void PlayerBullet::Directionoftravel() {
	// Y軸周り角度(θy)
	worldTarnsform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4x4 matrix = MakeRotateYMatrix(-worldTarnsform_.rotation_.y);
	// velocity_に回転行列を掛け算してvelocityZを求める
	Vector3 velocityZ = Multiply(velocity_, matrix);
	// X軸周り角度(θX)
	worldTarnsform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
}