#include "RailCamera.h"
#include "Geometry/fMatrix4x4.h"
#include "PrimitiveDrawer.h"
#include <imgui.h>
#include<cmath>

void RailCamera::Init(Vector3 pos, Vector3 rad) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	controlPoints_ = {
	    {0, 0, 0},
	    {10, 10,0, },
	    {10, 15, 0},
	    {20, 15, 0},
	    {20, 0, 0},
	    {30, 0, 0},
	};
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -3.14f, 3.14f);
	ImGui::End();
	

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;
	// 線分の数
	const size_t segmentCount = 100;
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
	
    	float t = 1.0f / segmentCount * i;
		float nextT = 1.0f / segmentCount * (i+1);
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		Vector3 nextPos = CatmullRomPosition(controlPoints_, nextT);
		eye_ = pos;
		target_ = nextPos;
		//回転----------
		forward_ = target_ - eye_;
		cameraRotate_.y = std::atan2(forward_.x, forward_.z);
		Matrix4x4 cameraRotateMatrix = MakeRotateYMatrix(-cameraRotate_.y);
		Vector3 velocityZ = Multiply(forward_,cameraRotateMatrix);
		cameraRotate_.x = std::atan2(-velocityZ.y, velocityZ.z);
		// ワールドからビュー
		viewProjection_.matView = Inverse(worldTransform_.matWorld_);

		// スケール、回転、平行移動を合成して行列を計算する
		worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, cameraRotate_, eye_);

		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}
}

void RailCamera::LineDraw() {

	// for (size_t i = 0; i < segmentCount; ++i) {
	//	// 先頭から2点取り出してライン描画
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], Vector4{1.0f, 0.0f, 0.0f, 1.0f});
	// }
}