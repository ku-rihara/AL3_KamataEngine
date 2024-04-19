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

	//キャラクター旋回処理
	Rotate();
	//キャラクターの移動処理
	Move();
	//キャラクターの攻撃処理
	Attack();
	//弾更新
	if (bullet_) {
		bullet_->Update();
	}
	
	worldTransform_.UpdateMatrix();//行列更新

	ImGui::Begin("player");
	ImGui::Text("PosX%f", worldTransform_.translation_.x);
	ImGui::Text("PosY%f", worldTransform_.translation_.y);
	ImGui::Text("PosZ%f", worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Move() {
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
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
	worldTransform_.translation_ += move;

	// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_W)) {
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(model_, worldTransform_.translation_);
		//弾を登録する
		bullet_=newBullet;
	}
}