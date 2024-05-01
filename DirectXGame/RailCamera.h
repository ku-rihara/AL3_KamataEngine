#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera {
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

public:
	
	void Init(Vector3 pos,Vector3 rad);
	void Update();

	//getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
};
