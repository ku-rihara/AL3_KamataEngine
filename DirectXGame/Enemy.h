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
		IndexThorn,
	};
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	std::vector<Model*> modelParts_;
	Model* modelHead_ = nullptr;
	Model* modelThorn_ = nullptr;

public:
	Enemy();

	void Init(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
	Vector3 GetBaseWorldPos()override;

	// setter
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
};


