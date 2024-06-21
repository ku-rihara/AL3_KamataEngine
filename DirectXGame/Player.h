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
	};
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	

	void Init(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseWorldPos()override;
	//浮遊ギミック
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
