#pragma once
#include"Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
class Colider {
private:
	//衝突半径
	float radius_ = 1.5f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

public:
	void Init();
	void UpdateWorldTransform();
	void Draw(Model* model, const ViewProjection& viewProjection);

	virtual ~Colider()=default;
	//中心座標を取得
	virtual Vector3 GetBaseCenterPosition() const = 0;
	//衝突時に呼ばれる関数
	virtual void OnCollision(){};

	//getter**********************************************
	//半径の取得
	float GetRadius() const { return radius_; }
	//setter**************************************************
	//半径を設定
	void SetRadius(float radius) { radius_ = radius; }
};
