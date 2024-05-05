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

	Vector3 eye_;
	Vector3 target_;
	Vector3 forward_;
	Vector3 cameraRotate_;

public:
	
	void Init(Vector3 pos,Vector3 rad);
	void Update();
	void LineDraw();

	//getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
};
