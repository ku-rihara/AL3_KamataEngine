#include "PlayerBullet.h"
#include "TextureManager.h"
#include"assert.h"


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