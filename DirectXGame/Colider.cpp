#include "Colider.h"

void Colider::Init() { 
	worldTransform_.Initialize(); 
}

void Colider::UpdateWorldTransform() {
	//ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ =GetBaseCenterPosition();
	worldTransform_.UpdateMatrix();
}

void Colider::Draw(Model* model, const ViewProjection& viewProjection) {
	//モデルの描画
	model->Draw(worldTransform_,viewProjection);
}