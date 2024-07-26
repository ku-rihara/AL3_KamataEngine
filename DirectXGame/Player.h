#pragma once
#include "BaseCharacter.h"
#include "GlobalParameter.h"
#include "JoyState.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include <optional>
#include <vector>

class LockOn;
class Player : public BaseCharacter {
private:
	enum Parts {
		IndexBody,
		IndexHead,
		IndexLeftArm,
		IndexRightArm,
		IndexWeapon,
	};

private:
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kDash,   // ダッシュ中
		kJump,   // ジャンプ中
	};

private:
	// 包含
	LockOn* lockOn_;
	//攻撃用定数
	struct ConstAttack {
		//振りかぶりの時間
		uint32_t anticipationTime;
		//ための時間
		uint32_t chargeTime;
		//攻撃振りの時間
		uint32_t swingTime;
		//硬直時間
		uint32_t recoveryTime;
		//振りかぶりの移動速さ
		float anticipationSpeed;
		//ための移動速さ
		float chargeSpeed;
		//攻撃振りの移動速さ
		float swingSpeed;
	};
	//コンボの数
	static const int comboNum = 3;
	//コンボ定数表
	static const std::array<ConstAttack, comboNum> kConstAttacks_;
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashPrameter_ = 0;
	};
	//攻撃用ワーク
	struct WorkAttack {
		uint32_t attackParameters_ = 0;
		int32_t comboIndex = 0;
		int32_t inComboPhase = 0;
		bool comboNext = false;
	};
	// パーツの数
	const int partsnum = 5;
	// 3Dモデル
	std::vector<Model*> modelParts_;

	float stiffeningTime_ = 0;
	float objectiveAngle_ = 0;
	WorkDash workDash_;
	WorkAttack workAttack_;

	// ふるまい
	Behavior behavior_ = Behavior::kRoot;
	GlobalParameter* globalParameter_;
	std::optional<Behavior> behaviorRequest_ = std::nullopt; // 次の振る舞いのリクエスト

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	float attackMoveT_ = 0;
	int32_t floatingCycle_;
	float floatingAmplitude_;

	// 速度
	Vector3 velocity_ = {};
	Vector3 attackDirection_ = {};
	float attackSpeed = {};
	Vector3 savePos_ = {};

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

public:
	Player();

	void Init(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseCenterPosition() const override;
	// ふるまい************************************************
	// 初期化----------------------------
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	void BehaviorDashInitialize();
	void BehaviorJumpInitialize();
	// 更新-------------------------------
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();
	void BehabiorDashUpdate();
	void BehaviorJumpUpdate();
	void Move(const float& speed);
	// アニメーション
	void AnimationInit() override;
	void AnimationUpdate() override;

	//コールバック関数
	void OnCollision() override;

	// 調節項目を適用
	void ApplyGlobalParameter();
	uint32_t GetSwingTime() const { return kConstAttacks_[workAttack_.comboIndex].swingTime; }
	uint32_t GetAnticipationTime() const { return kConstAttacks_[workAttack_.comboIndex].anticipationTime; }
	// setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetLockOn(LockOn* lockon) { lockOn_ = lockon; }
};
