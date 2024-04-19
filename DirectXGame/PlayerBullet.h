#pragma once

#include "Model.h"
#include "WorldTransform.h"
/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
private:
	//包含
	Model* model_;
	WorldTransform worldTarnsform_;
	uint32_t textureHandle_ = 0;

public:
	
	void Init(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};
