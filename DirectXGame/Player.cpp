#include "Player.h"
#include "Geometry/fMatrix4x4.h"
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
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move,rotateMatrix);
		// Y軸周り角度(θy)
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		// Y軸周りに-θy回す回転行列を計算
		Matrix4x4 matrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
		// velocity_に回転行列を掛け算してvelocityZを求める
		Vector3 velocityZ = Multiply(move, matrix);
		// X軸周り角度(θX)
		worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
		//移動
		worldTransform_.translation_ += move;
	}
	worldTransform_.UpdateMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }


Vector3 Player::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}