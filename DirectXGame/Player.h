#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"

class Player:public BaseCharacter {
private:
	enum Parts {
		IndexBody,
		IndexHead,
		IndexLeftArm,
		IndexRightArm,
		IndexWeapon,
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

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	float AttackEaseT_ = 0;
	
public:
	Player();
	
	void Init(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseWorldPos()override;
	//ふるまい
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();

	//アニメーション
	void AnimationInit() override;
	void AnimationUpdate() override;

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
