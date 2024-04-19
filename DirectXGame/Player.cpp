#include "Player.h"
#include"cassert"
#include"ImGuiManager.h"

Player::Player() {

}

Player::~Player() {
}

void Player::Init(Model* model, uint32_t textureHandle) {
	input_=Input::GetInstance();
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Player::Update() {
	Vector3 move = {0, 0, 0};
	//キャラクターの移動の速さ
	const float kCharacterSpeed=0.2f;
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_+=move;

		// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);


	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); 

	ImGui::Begin("player");
	ImGui::Text("PosX%f", worldTransform_.translation_.x);
	ImGui::Text("PosY%f", worldTransform_.translation_.y);
	ImGui::Text("PosZ%f", worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}