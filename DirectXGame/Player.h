#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"

class Player {
private:
	// ワールドトランスフォーム
	WorldTransform worldTransformformBase_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	

	void Init(std::vector<Model*> modelParts, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPos();

	const WorldTransform& GetWorldTransform()  { return worldTransformformBase_; }

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
