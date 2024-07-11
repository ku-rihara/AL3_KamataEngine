#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"
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
	};

	const int partsnum = 5;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;
	Model* modelWeapon_ = nullptr;
	float stiffeningTime_ = 0;
	float objectiveAngle_;
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次の振る舞いのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	float AttackEaseT_ = 0;
	
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
	//更新-------------------------------
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();

	//アニメーション
	void AnimationInit() override;
	void AnimationUpdate() override;

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
