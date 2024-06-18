#include "Player.h"
#include "Geometry/fMatrix4x4.h"
#include "cassert" 
#include "MathFunction.h"
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
		const float thresholdValue = 0.7f;
		float objectiveAngle = 0;
		bool isMoving = false;
		//移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		if (Length(move) > thresholdValue) {
			isMoving = true;
		}
		if (isMoving) {
			// 移動量に速さを反映
			move = Normnalize(move) * speed;
			// 移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);
			// 移動
			worldTransform_.translation_ += move;
			// 目標角度
			 objectiveAngle = std::atan2(move.x, move.z);
		
		}
			//最短角度補間
			worldTransform_.rotation_.y =LerpShortAngle(worldTransform_.rotation_.y,objectiveAngle,0.05f);		
			
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