#include "Player.h"
#include "Geometry/fMatrix4x4.h"
#include<imgui.h>
#include "cassert"
#include"Easing.h"

Player::Player() {}
float pi = 3.14159265358f;
void Player::Init(const std::vector<Model*>& models) {

	partsWorldTransforms_.reserve(partsnum); // メモリを確保
	for (int i = 0; i < partsnum; ++i) {
		partsWorldTransforms_.emplace_back(std::make_unique<WorldTransform>());
	}	
	// 基底クラスの初期化
	BaseCharacter::Init(models);
	//パーツの親子関係
	partsWorldTransforms_[IndexBody]->parent_ = &baseWorldTransform_;
	partsWorldTransforms_[IndexHead]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexLeftArm]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexRightArm]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexWeapon]->parent_ = partsWorldTransforms_[IndexBody].get();
	//パーツの変位の値
	baseWorldTransform_.translation_.y = 0.9f;
	partsWorldTransforms_[IndexHead]->translation_.y = 1.7f;
	partsWorldTransforms_[IndexLeftArm]->translation_.x = 0.6f;
	partsWorldTransforms_[IndexLeftArm]->translation_.y = 1.0f;
	partsWorldTransforms_[IndexRightArm]->translation_.x = -0.6f;
	partsWorldTransforms_[IndexRightArm]->translation_.y = 1.0f;
	BehaviorRootInitialize();
}

void Player::Update() {

	if (behaviorRequest_) {
	//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_) { 
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();			
			break;
		
		case Behavior::kAttack:
			BehaviorAttackInitialize();		
			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:	
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	BaseCharacter::Update();
}
void Player::Draw(const ViewProjection& viewProjection) {
	BaseCharacter::Draw(viewProjection); 
}
/*関数*/
void Player::AnimationUpdate() {
	
	// 浮遊移動のサイクル
	const uint16_t cycle = 70;
	//1フレームでのパラメータ加算値
	const float step = 2.0f * float(pi) / cycle;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	//浮遊の振幅＜m＞
	const float floatingAmplitude = 0.2f;
	//浮遊を座標に反映
	partsWorldTransforms_[IndexBody]->translation_.y = std::sin(floatingParameter_) * floatingAmplitude;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &partsWorldTransforms_[IndexHead]->translation_.x, 0, 2.0f);
	ImGui::SliderFloat3("ArmL Translation", &partsWorldTransforms_[IndexLeftArm]->translation_.x, 0, 2.0f);
	ImGui::SliderFloat3("ArmR Translation", &partsWorldTransforms_[IndexRightArm]->translation_.x, -1, 2.0f);
	ImGui::End();
}

void Player::BehaviorRootUpdate() {
	AnimationUpdate();
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
		baseWorldTransform_.rotation_.y = std::atan2(move.x, move.z);
		// Y軸周りに-θy回す回転行列を計算
		Matrix4x4 matrix = MakeRotateYMatrix(-baseWorldTransform_.rotation_.y);
		// velocity_に回転行列を掛け算してvelocityZを求める
		Vector3 velocityZ = Multiply(move, matrix);
		// X軸周り角度(θX)
		baseWorldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
		// 移動
		baseWorldTransform_.translation_ += move;
	}

if(joyState.Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER){
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackUpdate() {

	 /*浮遊移動のサイクル*/
	AttackEaseT_ += 0.05f;
	if (AttackEaseT_ >= 1.0f) {
		AttackEaseT_ = 1.0f;
		stiffeningTime_++;
		if (stiffeningTime_ >= 10) {
			behaviorRequest_ = Behavior::kRoot;
		}
	}

	// 回転する
	partsWorldTransforms_[IndexWeapon]->rotation_.x = Lerp(-pi / 3, pi / 2, AttackEaseT_);
	partsWorldTransforms_[IndexRightArm]->rotation_.x = Lerp(2.4f, 5.0f, AttackEaseT_);
	partsWorldTransforms_[IndexLeftArm]->rotation_.x = Lerp(2.4f, 5.0f, AttackEaseT_);

	ImGui::Begin("ATK");
	ImGui::DragFloat("EaseT", &AttackEaseT_, 0.01f);
	ImGui::DragFloat3("Weapon", &partsWorldTransforms_[IndexWeapon]->rotation_.x, 0.01f);
	ImGui::DragFloat3("Left", &partsWorldTransforms_[IndexLeftArm]->rotation_.x, 0.01f);
	ImGui::DragFloat3("Right", &partsWorldTransforms_[IndexRightArm]->rotation_.x, 0.01f);
	ImGui::End();
}

//通常初期化
void Player::BehaviorRootInitialize() { 
	partsWorldTransforms_[IndexWeapon]->scale_ = {};
	partsWorldTransforms_[IndexLeftArm]->rotation_ = {0,0,0};
	partsWorldTransforms_[IndexRightArm]->rotation_ = {0, 0, 0};
	AnimationInit();
}
//アタック初期化
void Player::BehaviorAttackInitialize() { 
	partsWorldTransforms_[IndexWeapon]->scale_ = {1,1,1};
	stiffeningTime_ = 0;
	AttackEaseT_ = 0; 
}

void Player::AnimationInit() { floatingParameter_ = 0.0f; }

Vector3 Player::GetBaseWorldPos() { return BaseCharacter::GetBaseWorldPos(); }