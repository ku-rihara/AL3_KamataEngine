#include "Enemy.h"
#include "Geometry/fMatrix4x4.h"
#include<imgui.h>
#include "cassert"


Enemy::Enemy() {}

void Enemy::Init(const std::vector<Model*>& models) {

	partsWorldTransforms_.reserve(2); // メモリを確保
	for (int i = 0; i < 2; ++i) {
		partsWorldTransforms_.emplace_back(std::make_unique<WorldTransform>()); 
	}
	// 基底クラスの初期化
	BaseCharacter::Init(models);
	// パーツの親子関係
	partsWorldTransforms_[IndexHead]->parent_ = &baseWorldTransform_;
	partsWorldTransforms_[IndexThorn]->parent_ = partsWorldTransforms_[IndexHead].get();

	
}

void Enemy::Update() {
	
	BaseCharacter::Update();
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	BaseCharacter::Draw(viewProjection);
}


Vector3 Enemy::GetBaseWorldPos() { return BaseCharacter::GetBaseWorldPos(); }