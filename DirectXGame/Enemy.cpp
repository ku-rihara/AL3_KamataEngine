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
	baseWorldTransform_.translation_.x = -20;
	baseWorldTransform_.translation_.z = 20;
	partsWorldTransforms_[IndexLeftThorn]->translation_.x = 0.7f;
	partsWorldTransforms_[IndexRightThorn]->translation_.x = -0.7f;
	partsWorldTransforms_[IndexLeftThorn]->translation_.y = 0.4f;
	partsWorldTransforms_[IndexRightThorn]->translation_.y = 0.4f;
	// パーツの親子関係
	partsWorldTransforms_[IndexHead]->parent_ = &baseWorldTransform_;
	partsWorldTransforms_[IndexLeftThorn]->parent_ = partsWorldTransforms_[IndexHead].get();
	partsWorldTransforms_[IndexRightThorn]->parent_ = partsWorldTransforms_[IndexHead].get();
}

void Enemy::Update() {
	const float speed = 0.1f;
	AnimationUpdate();
	baseWorldTransform_.rotation_.y += 0.01f;
	Vector3 forwardDirection = { std::sinf(baseWorldTransform_.rotation_.y), 0, std::cosf(baseWorldTransform_.rotation_.y)};
	velocity_ = forwardDirection * speed;
	baseWorldTransform_.translation_ += velocity_;

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


void Enemy::AnimationInit() { 
	// 浮遊ギミックの媒介変数
	 animeParamater_ = 0.0f;
}

void Enemy::AnimationUpdate() {
	float pi = 3.14159265358f;
	// 浮遊移動のサイクル
	const uint16_t cycle = 10;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(pi) / cycle;
	// パラメータを1ステップ分加算
	animeParamater_ += step;
	animeParamater_ = std::fmod(animeParamater_, 2.0f * pi);
	// 浮遊の振幅＜m＞
	const float floatingAmplitude = 0.2f;
	// 浮遊を座標に反映

	partsWorldTransforms_[IndexLeftThorn]->translation_.y = 0.3f+std::sin(animeParamater_) * floatingAmplitude;
	partsWorldTransforms_[IndexLeftThorn]->translation_.z = std::cos(animeParamater_) * floatingAmplitude;
	partsWorldTransforms_[IndexRightThorn]->translation_.y = 0.3f + std::sin(animeParamater_) * floatingAmplitude;
	partsWorldTransforms_[IndexRightThorn]->translation_.z = std::cos(animeParamater_) * floatingAmplitude;
}

Vector3 Enemy::GetBaseWorldPos() { return BaseCharacter::GetBaseWorldPos(); }