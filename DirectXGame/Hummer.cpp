#include "Hummer.h"

float pi = 3.14159265358f;

void Hummer::SetParent(const WorldTransform& worldTransform) { 
	worldTransform_.parent_ = &worldTransform;
}

void Hummer::Init(const Model& models) { 
	model_ = models;
}

void Hummer::Update() { 
	worldTransform_.rotation_.x = Lerp(-pi / 3, pi / 2, AttackEaseT_); 
}

void Hummer::Draw(const ViewProjection& viewProjection) { 
	model_->Draw() 
}