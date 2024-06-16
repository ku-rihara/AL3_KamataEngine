#pragma once
#include <memory>
#include <vector>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/input.h"

class Player {
private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;
	// ワールド変換データ
	/*std::vector<WorldTransform> worldTransforms;*/
	WorldTransform worldTransformformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformformleftArm_;
	WorldTransform worldTransformformRightArm_;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	

	void Init(Model* modelBody, Model* modelHead, Model* modelLeftArm, Model* modelRightArm);
	void Update();
	void Draw(ViewProjection& viewProjection);
	//浮遊ギミック
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	Vector3 GetWorldPos();

	const WorldTransform& GetWorldTransform()  { return worldTransformformBase_; }

	//setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
};
