#include "Enemy.h"
#include "Geometry/fMatrix4x4.h"
#include<imgui.h>
#include "cassert"

Enemy::Enemy() {}

void Enemy::Init(const std::vector<Model*>& models) {

	partsWorldTransforms_.reserve(partsnum); // メモリを確保
	for (int i = 0; i < partsnum; ++i) {
		partsWorldTransforms_.emplace_back(std::make_unique<WorldTransform>()); 
	}
	// 基底クラスの初期化
	BaseCharacter::Init(models);
	baseWorldTransform_.translation_.x = 2;
	baseWorldTransform_.translation_.y = 2;
	partsWorldTransforms_[IndexLeftThorn]->translation_.x = 0.7f;
	partsWorldTransforms_[IndexRightThorn]->translation_.x = -0.7f;
	partsWorldTransforms_[IndexLeftThorn]->translation_.y = 0.25f;
	partsWorldTransforms_[IndexRightThorn]->translation_.y = 0.25f;
	// パーツの親子関係
	partsWorldTransforms_[IndexHead]->parent_ = &baseWorldTransform_;
	partsWorldTransforms_[IndexLeftThorn]->parent_ = partsWorldTransforms_[IndexHead].get();
	partsWorldTransforms_[IndexRightThorn]->parent_ = partsWorldTransforms_[IndexHead].get();
}

void Enemy::Update() {
	
	BaseCharacter::Update();
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Head Translation", &partsWorldTransforms_[IndexHead]->translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Translation", &partsWorldTransforms_[IndexLeftThorn]->translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Translation", &partsWorldTransforms_[IndexRightThorn]->translation_.x, 0.01f);
	ImGui::End();
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	BaseCharacter::Draw(viewProjection);
}


Vector3 Enemy::GetBaseWorldPos() { return BaseCharacter::GetBaseWorldPos(); }