#include "RailCamera.h"
#include "Geometry/fMatrix4x4.h"
#include <imgui.h>

void RailCamera::Init(Vector3 pos, Vector3 rad) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;

	viewProjection_.Initialize();
}

void RailCamera::Update() {
	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -3.14f, 3.14f);
	ImGui::End();
	//worldTransform_.translation_.z -= 0.12f;
	//worldTransform_.rotation_.y += 0.001f;
	
	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// ワールドからビュー
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}