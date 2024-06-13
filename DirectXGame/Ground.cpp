#include "Ground.h"

#include "assert.h"

Ground::Ground() {}

Ground::~Ground() {}

void Ground::Init(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
}

void Ground::Update() { worldTransform_.UpdateMatrix(); }

void Ground::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }
