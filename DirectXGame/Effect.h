#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"
class Effect {
private:
	// モデル配列データ
	Model* model_;
	// ワールド変換データ
	WorldTransform baseWorldTransform_;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

public:
	void Init();
	void Update();
	void Draw(const ViewProjection&viewProjection);

};
