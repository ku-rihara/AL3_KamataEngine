#include "Skydome.h"
#include"assert.h"

Skydome::Skydome() {

}

Skydome::~Skydome() {}

void Skydome::Init(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {100, 100, 100};
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}

