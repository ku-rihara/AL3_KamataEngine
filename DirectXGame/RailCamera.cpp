#include "RailCamera.h"
#include "Easing.h"
#include "Geometry/fMatrix4x4.h"
#include "PrimitiveDrawer.h"
#include <cmath>
#include <imgui.h>
#include "Input.h"

void RailCamera::Init(Vector3 pos, Vector3 rad) {
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	controlPoints_ = {
	    {0, 0, 0},
	    {10, 10, 3,},
	    {10, 15, 0},
	    {20, 15, 0},
	    {20, 0, 6},
	    {30, 0, 0},
	};
	viewProjection_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void RailCamera::Update() {
	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -3.14f, 3.14f);
	ImGui::DragInt("Index", &RailIndex);
	ImGui::End();

		// 線分の数+1個分の頂点座標を計算
		for (size_t i = 0; i < segmentCount + 1; i++) {
			float t = 1.0f / segmentCount * i;
			Vector3 pos = CatmullRomPosition(controlPoints_, t);
			// 描画用頂点リストに追加
			pointsDrawing.push_back(pos);
		}
	   
		railMoveTime_ += 0.6f;
		if (RailIndex < segmentCount) {
			Vector3 eye = pointsDrawing[RailIndex];
			Vector3 target = pointsDrawing[RailIndex + 1];
			Vector3 forward = target - eye;

			cameraRotate_.y = std::atan2(forward.x, forward.z);
			Matrix4x4 cameraRotateMatrix = MakeRotateYMatrix(-cameraRotate_.y);
			Vector3 velocityZ = Multiply(forward, cameraRotateMatrix);
			cameraRotate_.x = std::atan2(-velocityZ.y, velocityZ.z);

			worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, cameraRotate_, Lerp(eye, target, railMoveTime_));
			viewProjection_.matView = Inverse(worldTransform_.matWorld_);
			if (railMoveTime_ >= 1.0f) {
				railMoveTime_ = 0;
				RailIndex++;
			}
		}
	
}

void RailCamera::LineDraw() {

	for (size_t i = 0; i < segmentCount; ++i) {
		// 先頭から2点取り出してライン描画
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], Vector4{1.0f, 0.0f, 0.0f, 1.0f});
	}
}



