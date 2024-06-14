#include "FollowCamera.h"
#include "input/input.h"
#include"Geometry/fMatrix4x4.h"
void FollowCamera::Init() { viewprojection_.Initialize(); }


void FollowCamera::Update() {
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotateSpeed=0.1f;
		viewprojection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
	}

	//追従対象がいれば
	if (target_) {
	//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};
	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewprojection_.rotation_.y);
		offset = TransformNormal(offset, rotateMatrix);
		//座標をコピーしてオフセットはずらす
		viewprojection_.translation_ = target_->translation_ + offset;
	}
	//ビュー行列の更新
	viewprojection_.UpdateMatrix();

}