#include "Player.h"
#include "Geometry/fMatrix4x4.h"
#include "cassert"

Player::Player() {}

void Player::Init(Model* modelBody, Model* modelHead, Model* modelLeftArm, Model* modelRightArm) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelLeftArm);
	assert(modelRightArm);
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelLeftArm_ = modelLeftArm;
	modelRightArm_ = modelRightArm;
	worldTransformformBase_.Initialize();
	worldTransformBody_. Initialize();
	worldTransformHead_. Initialize();
	worldTransformformleftArm_.Initialize();
	worldTransformformRightArm_.Initialize();
	worldTransformBody_.parent_ = &worldTransformformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformformleftArm_.parent_ = &worldTransformBody_;
	worldTransformformRightArm_.parent_ = &worldTransformBody_;
	InitializeFloatingGimmick();
}

void Player::Update() {
	UpdateFloatingGimmick();
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		// 移動量に速さを反映
		move = Normnalize(move) * speed;
		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, rotateMatrix);
		// Y軸周り角度(θy)
		worldTransformformBase_.rotation_.y = std::atan2(move.x, move.z);
		// Y軸周りに-θy回す回転行列を計算
		Matrix4x4 matrix = MakeRotateYMatrix(-worldTransformformBase_.rotation_.y);
		// velocity_に回転行列を掛け算してvelocityZを求める
		Vector3 velocityZ = Multiply(move, matrix);
		// X軸周り角度(θX)
		worldTransformformBase_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
		// 移動
		worldTransformformBase_.translation_ += move;
	}
	worldTransformformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformformleftArm_.UpdateMatrix();
	worldTransformformRightArm_.UpdateMatrix();
}
void Player::Draw(ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection); 
	modelHead_->Draw(worldTransformHead_, viewProjection );
	modelLeftArm_->Draw(worldTransformformleftArm_, viewProjection );
	modelRightArm_->Draw(worldTransformformRightArm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick(){
	float pi = 3.14159265358f;
	// 浮遊移動のサイクル
	const uint16_t cycle = 60;
	//1フレームでのパラメータ加算値
	const float step = 2.0f * float(pi) / cycle;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	//浮遊の振幅＜m＞
	const float floatingAmplitude = 1.2f;
	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;
}

Vector3 Player::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransformformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformformBase_.matWorld_.m[3][2];

	return worldPos;
}