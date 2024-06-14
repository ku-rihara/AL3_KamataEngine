#include "Player.h"
#include "cassert" 

Player::Player() {}

void Player::Init(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Player::Update() {
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		//移動量に速さを反映
		move = Normnalize(move) * speed;
		//移動ベクトルをカメラの角度だけ回転する
		move += viewProjection_->rotation_;
		//移動
		worldTransform_.translation_ += move;

	}
	worldTransform_.UpdateMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }