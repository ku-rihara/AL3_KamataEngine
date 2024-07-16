#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"
#include "GlobalParameter.h"
#include <optional>


class Player:public BaseCharacter {
private:
	enum Parts {
		IndexBody,
		IndexHead,
		IndexLeftArm,
		IndexRightArm,
		IndexWeapon,
	};
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kDash,//ダッシュ中
	};
	//ダッシュ用ワーク
	struct WorkDash {
		//ダッシュ用の媒介変数
		uint32_t dashPrameter_ = 0;
	};

	const int partsnum = 5;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;

	float stiffeningTime_ = 0;
	float objectiveAngle_=0;
	WorkDash workDash_;
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	GlobalParameter* globalParameter_;
	//次の振る舞いのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	float AttackEaseT_ = 0;
	int32_t floatingCycle_;
	 float floatingAmplitude_;

public:
	Player();
	
	void Init(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseWorldPos()override;
	//ふるまい************************************************
	//初期化----------------------------
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	void BehaviorDashInitialize();
	//更新-------------------------------
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();
	void BehabiorDashUpdate();
	void Move(const float& speed);
	//アニメーション
	void AnimationInit() override;
	void AnimationUpdate() override;

	//調節項目を適用
	void ApplyGlobalParameter();
	
	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
