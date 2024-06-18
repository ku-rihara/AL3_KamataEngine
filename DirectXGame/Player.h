#pragma once
#include <memory>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"

class Player {
private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	 Model* model_ = nullptr;
	float objectiveAngle_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	

	void Init(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPos();

	const WorldTransform& GetWorldTransform()  { return worldTransform_; }

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
