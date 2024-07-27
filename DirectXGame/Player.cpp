#include "Player.h"
#include "Easing.h"
#include "MathFunction.h"
#include "Matrix4x4.h"
#include "cassert"
#include <imgui.h>
// class
#include "LockOn.h"

const std::array<Player::ConstAttack, Player::comboNum> Player::kConstAttacks_ = {
    {
     //振りかぶり、攻撃前硬直、攻撃振り時間、個直、各フェーズの移動速さ
     {0, 0, 20, 0, 0.0f, 0.0f, 0.15f},
     {15, 10, 25, 0, 0.2f, 0.0f, 0.0f},
     {15, 10, 25, 30, 0.2f, 0.0f, 0.0f},
     }
};

float pi = 3.14159265358f;

Player::Player() {}

void Player::Init(const std::vector<Model*>& models) {

	partsWorldTransforms_.reserve(partsnum); // メモリを確保
	for (int i = 0; i < partsnum; ++i) {
		partsWorldTransforms_.emplace_back(std::make_unique<WorldTransform>());
	}
	// 基底クラスの初期化
	BaseCharacter::Init(models);
	// パーツの親子関係
	partsWorldTransforms_[IndexBody]->parent_ = &baseWorldTransform_;
	partsWorldTransforms_[IndexHead]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexLeftArm]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexRightArm]->parent_ = partsWorldTransforms_[IndexBody].get();
	partsWorldTransforms_[IndexWeapon]->parent_ = &baseWorldTransform_;
	// パーツの変位の値
	baseWorldTransform_.translation_.y = 0.9f;


	BehaviorRootInitialize();
	globalParameter_ = GlobalParameter::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	globalParameter_->CreateGroup(groupName);
	/*globalParameter_->AddItem(groupName, "Test", 90);*/
	globalParameter_->AddItem(groupName, "Head Translation", partsWorldTransforms_[IndexHead]->translation_);
	globalParameter_->AddItem(groupName, "ArmL Translation", partsWorldTransforms_[IndexLeftArm]->translation_);
	globalParameter_->AddItem(groupName, "ArmR Translation", partsWorldTransforms_[IndexRightArm]->translation_);
	globalParameter_->AddItem(groupName, "HummerRotate", partsWorldTransforms_[IndexWeapon]->rotation_);
	globalParameter_->AddItem(groupName, "floatingCycle", floatingCycle_);
	globalParameter_->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
}

void Player::Update() {
	ApplyGlobalParameter();
	
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// 振る舞い更新
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehabiorDashUpdate();
		break;
	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}
	
	BaseCharacter::Update();
}
void Player::Draw(const ViewProjection& viewProjection) { BaseCharacter::Draw(viewProjection); }
/*関数*/
void Player::AnimationUpdate() {

	// 浮遊移動のサイクル
	// floatingCycle_ = 70;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(pi) / floatingCycle_;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	// 浮遊の振幅＜m＞
	/*floatingAmplitude_ = 0.2f;*/
	// 浮遊を座標に反映
	partsWorldTransforms_[IndexBody]->translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;

	
}
//通常モード更新
void Player::BehaviorRootUpdate() {

	Move(0.3f);

	// Rで攻撃
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kAttack;
	}
	// Lでダッシュ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		behaviorRequest_ = Behavior::kDash;
	}
	// 一旦Jでジャンプ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		behaviorRequest_ = Behavior::kJump;
	}
}
//攻撃更新
void Player::BehaviorAttackUpdate() {
	//ターゲット追い越し防止
	if (lockOn_ && lockOn_->GetEnemyTarget()) {
		Vector3 differectialVector = lockOn_->GetTargetPosition() - GetBaseCenterPosition();
		// 距離
		float distance = Length(differectialVector);
		// 距離しきい値
		const float threshold = 0.2f;
		// しきい値より離れている時
		if (distance > threshold) {
			// Y軸回り角度
			baseWorldTransform_.rotation_.y = std::atan2(differectialVector.x, differectialVector.z);
			// しきい値を超える速さなら補正する
			if (attackSpeed > distance - threshold) {
				attackSpeed = distance - threshold;
			}
		}
	}
	// 動作時間
	uint32_t swingTime = kConstAttacks_[workAttack_.comboIndex].swingTime;
	//硬直時間
	uint32_t waitTime = kConstAttacks_[workAttack_.comboIndex].recoveryTime;
	Vector3 attackPos = attackDirection_ * attackSpeed;

	//コンボ上限に達していない
	if (workAttack_.comboIndex < comboNum-1) {
		if (Input::GetInstance()->GetJoystickState(0, joyState) && Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre)) {
			//攻撃ボタンをトリガーしたら
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && joyStatePre.Gamepad.wButtons != XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			//コンボ有効
				workAttack_.comboNext = true;
			}
		}
	}
	//既定の時間経過で通常攻撃に戻る
	if (workAttack_.attackParameters_ + workAttack_.stopParameters_ >= swingTime + waitTime) {
		//コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext) {
			workAttack_.comboNext = false;
		
				workAttack_.comboIndex++;
			
			BehaviorAttackInitialize();
		} else {
			workAttack_.comboIndex = 0;
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	
	if (workAttack_.attackParameters_ < swingTime) {
		workAttack_.attackParameters_++; 
		
	} else {
		workAttack_.stopParameters_++;
	}
	//コンボ段階によってモーションを分岐
	switch (workAttack_.comboIndex) {
	
	case 0:
		// 0:右から半時計回り
		Move(kConstAttacks_[workAttack_.comboIndex].swingSpeed);
		partsWorldTransforms_[IndexWeapon]->rotation_.x = Lerp(0.7f, 2.41f, float(workAttack_.attackParameters_) / float(kConstAttacks_[workAttack_.comboIndex].swingTime));	
		partsWorldTransforms_[IndexWeapon]->rotation_.y = -0.1f;
		partsWorldTransforms_[IndexWeapon]->rotation_.z = -1.56f;	
		break;
	case 1:
		//上から降り下ろし
		//  攻撃先の座標を決める
		attackMoveT_ += 0.1f;
		 if (attackMoveT_ >= 1.0f) {
			attackMoveT_ = 1.0f;
		}
		 ////振りかぶり
		 //if (workAttack_.attackParameters_ <= GetAnticipationTime()) {
		 //}
		baseWorldTransform_.translation_ = Lerp(baseWorldTransform_.translation_, savePos_ + attackPos, attackMoveT_);
		// 回転する
		partsWorldTransforms_[IndexWeapon]->rotation_.x = Lerp(-pi / 4.0f, 3.0f * pi / 7.0f, float(workAttack_.attackParameters_) / float(swingTime));
		partsWorldTransforms_[IndexRightArm]->rotation_.x = Lerp(2.4f, 5.0f, float(workAttack_.attackParameters_) / float(swingTime));
		partsWorldTransforms_[IndexLeftArm]->rotation_.x = Lerp(2.4f, 5.0f, float(workAttack_.attackParameters_) / float(swingTime));
		break;
	case 2:
	default:
		//右からホームラン
		partsWorldTransforms_[IndexWeapon]->rotation_.x = Lerp(0.5f, 6.8f, float(workAttack_.attackParameters_) / float(swingTime));
		partsWorldTransforms_[IndexWeapon]->rotation_.y = -0.1f;
		partsWorldTransforms_[IndexWeapon]->rotation_.z = -1.56f;	
		partsWorldTransforms_[IndexBody]->rotation_.y = Lerp(pi / 2.0f, -3.0f * pi / 2.0f, float(workAttack_.attackParameters_) / float(swingTime));
		break;

	}

	//workAttack_.attackParameters_ += 1.0f;
	//attackMoveT_ += 0.1f;
	//if (attackMoveT_ >= .0f) {
	//	attackMoveT_ = 1.0f;
	//}
	//if (workAttack_.attackParameters_ >= 1.0f) {
	//	workAttack_.attackParameters_ = 1.0f;
	//	stiffeningTime_++;
	//	if (stiffeningTime_ >= 10) {
	//		behaviorRequest_ = Behavior::kRoot;
	//	}
	//}

}
// ダッシュ更新
void Player::BehabiorDashUpdate() {
	Move(2.7f);
	// ダッシュの時間
	const uint32_t behaviorDashTime = 120;

	// 既定の時間経過で通常行動に戻る
	if (++workDash_.dashPrameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpUpdate() {

	// 移動
	baseWorldTransform_.translation_ += velocity_;
	// 重力加速度
	const float kGravityAcceleration = 0.05f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	// 加速する
	velocity_ += accelerationVector;

	// 着地
	if (baseWorldTransform_.translation_.y <= 0.0f) {
		baseWorldTransform_.translation_.y = 0.9f;
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::Move(const float& speed) {
	AnimationUpdate();
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float thresholdValue = 0.7f;
		bool isMoving = false;
		// 移動量
		velocity_ = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		if (Length(velocity_) > thresholdValue) {
			isMoving = true;
		}
		if (isMoving) {
			// 移動量に速さを反映
			velocity_ = Normnalize(velocity_) * speed;
			// 移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);
			// 移動
			baseWorldTransform_.translation_ += velocity_;
			// 目標角度
			objectiveAngle_ = std::atan2(velocity_.x, velocity_.z);
			// 最短角度補間
			baseWorldTransform_.rotation_.y = LerpShortAngle(baseWorldTransform_.rotation_.y, objectiveAngle_, 0.3f);

		} else if (lockOn_ && lockOn_->GetEnemyTarget()) {
			Vector3 differectialVector = lockOn_->GetTargetPosition() - GetBaseCenterPosition();

			// Y軸周り角度(θy)
			baseWorldTransform_.rotation_.y = std::atan2(differectialVector.x, differectialVector.z);
		}
	}
}

// 通常初期化
void Player::BehaviorRootInitialize() {
	partsWorldTransforms_[IndexWeapon]->scale_ = {};
	partsWorldTransforms_[IndexBody]->rotation_ = {0, 0, 0};
	partsWorldTransforms_[IndexLeftArm]->rotation_ = {0, 0, 0};
	partsWorldTransforms_[IndexRightArm]->rotation_ = {0, 0, 0};
	AnimationInit();
}
// アタック初期化
void Player::BehaviorAttackInitialize() {
	//攻撃モーションパラメータ初期化
	stiffeningTime_ = 0;
	workAttack_.attackParameters_ = 0;
	workAttack_.stopParameters_ = 0;
	attackMoveT_ = 0;
	//各パーツの初期化
	partsWorldTransforms_[IndexWeapon]->scale_ = {1, 1, 1};
	partsWorldTransforms_[IndexBody]->rotation_ = {0, 0, 0};
	partsWorldTransforms_[IndexLeftArm]->rotation_ = {0, 0, 0};
	partsWorldTransforms_[IndexRightArm]->rotation_ = {0, 0, 0};
	partsWorldTransforms_[IndexWeapon]->rotation_ = {0, 0, 0};
	//向いている方向を計算
	Matrix4x4 rotateMatrix = MakeRotateYMatrix(baseWorldTransform_.rotation_.y);
	Vector3 forward = {0, 0, 1};
	Vector3 direction = TransformNormal(forward, rotateMatrix);
	savePos_ = baseWorldTransform_.translation_;
	attackDirection_ = Normnalize(direction);
	//スピードパラメータの設定
	attackSpeed = 5.0f;
}
// ダッシュ初期化
void Player::BehaviorDashInitialize() {
	workDash_.dashPrameter_ = 0;
	baseWorldTransform_.rotation_.y = objectiveAngle_;
}

void Player::BehaviorJumpInitialize() {
	partsWorldTransforms_[IndexBody]->translation_.y = 0;
	partsWorldTransforms_[IndexLeftArm]->rotation_.x = 0;
	partsWorldTransforms_[IndexRightArm]->rotation_.x = 0;
	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	// ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;
}

// アニメーション初期化
void Player::AnimationInit() { floatingParameter_ = 0.0f; }

Vector3 Player::GetBaseCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, baseWorldTransform_.matWorld_);
	return worldPos;
}

void Player::ApplyGlobalParameter() {

	GlobalParameter* globalParameter = GlobalParameter::GetInstance();
	const char* groupName = "Player";
	partsWorldTransforms_[IndexHead]->translation_ = globalParameter->GetValue<Vector3>(groupName, "Head Translation");
	partsWorldTransforms_[IndexLeftArm]->translation_ = globalParameter->GetValue<Vector3>(groupName, "ArmL Translation");
	partsWorldTransforms_[IndexRightArm]->translation_ = globalParameter->GetValue<Vector3>(groupName, "ArmR Translation");
	partsWorldTransforms_[IndexWeapon]->rotation_ = globalParameter->GetValue<Vector3>(groupName, "HummerRotate");
	floatingCycle_ = globalParameter->GetValue<int32_t>(groupName, "floatingCycle");
	floatingAmplitude_ = globalParameter->GetValue<float>(groupName, "floatingAmplitude");
}

void Player::OnCollision() {
	// ジャンプリクエスト
	behaviorRequest_ = Behavior::kJump;
}