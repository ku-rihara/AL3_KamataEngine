#include "FollowCamera.h"

void FollowCamera::Init() { viewprojection_.Initialize(); }


void FollowCamera::Update() {
//追従対象がいれば
	if (target_) {
	//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		//座標をコピーしてオフセットはずらす
		viewprojection_.translation_ = target_->translation_ + offset;
	}
	//ビュー行列の更新
	viewprojection_.UpdateMatrix();

}