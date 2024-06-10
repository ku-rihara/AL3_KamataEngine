#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathFunction.h"
class RailCamera {
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_;
	// 線分の数
	const size_t segmentCount = 300;
	float railMoveTime_;
	Vector3 cameraRotate_;
	int RailIndex = 0;
	// 線分で描画する用の頂点リスト
	std::vector<Vector3> eye_;
	std::vector<Vector3> target_;
	std::vector<Vector3> forward_;
	std::vector<Vector3> pointsDrawing;
	

public:
	
	void Init(Vector3 pos,Vector3 rad);
	void Update();
	void LineDraw();

	//getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
};
