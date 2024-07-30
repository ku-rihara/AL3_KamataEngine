#include "Effect.h"
#include"Easing.h"

void Effect::Init(Model* model, const Vector3& Pos) { 
	model_ = model;
	worldTransform_.translation_ = Pos;
	easeT_ = 0.0f;
	isFinished_ = false;
	worldTransform_.Initialize();
}

void Effect::Update() {
	easeT_ += 0.05f;

	if (easeT_ >= 1.0f) {
		easeT_ = 1.0f;
		isFinished_ = true;
	}

	worldTransform_.scale_ = Lerp({}, Vector3(1, 1, 1), easeT_);
}
void Effect::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection); 
}

bool Effect::IsFinished() const { return isFinished_; }