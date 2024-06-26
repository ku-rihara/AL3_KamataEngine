#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class Enemy:public BaseCharacter {
private:
	enum Parts {
		IndexHead,
		IndexLeftThorn,
		IndexRightThorn,
	};
	const int partsnum = 3;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Vector3 velocity_;
	Model* modelHead_ = nullptr;
	Model* modelThorn_ = nullptr;
	// 浮遊ギミックの媒介変数
	float animeParamater_ = 0.0f;

public:
	Enemy();

	void Init(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseWorldPos()override;
	void AnimationInit()override;
	void AnimationUpdate()override;

	// setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
};


