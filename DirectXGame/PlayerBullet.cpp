#include "PlayerBullet.h"
#include "TextureManager.h"
#include"assert.h"


void PlayerBullet::Init(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTarnsform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTarnsform_.translation_ = position;
}

void PlayerBullet::Update() { 
	worldTarnsform_.UpdateMatrix(); 

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//モデルの描画
	model_->Draw(worldTarnsform_, viewProjection, textureHandle_);
}