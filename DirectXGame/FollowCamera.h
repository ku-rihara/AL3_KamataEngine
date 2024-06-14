#pragma once
#include"ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
private:
	ViewProjection viewprojection_;
	//追従対象
const	WorldTransform* target_ = nullptr;

public:
	void Init();
	void Update();


	//getter
	const ViewProjection& GetViewProjection()  { return viewprojection_; }
	//setter
	void SetTarget(const WorldTransform* target) { target_ = target; }
};
