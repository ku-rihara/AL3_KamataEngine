#include "BaseCharacter.h"

void BaseCharacter::Init(const std::vector<Model*>& models) {
	models_ = models; 
	for (WorldTransform& worldTransform : partsWorldTransforms_) {
		worldTransform.Initialize();
	}
}

void BaseCharacter::Update() {
	for (WorldTransform& worldTransform : partsWorldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void BaseCharacter::Draw(const ViewProjection&viewProjecion) { 

for (int i = 0; i < models_.size();i++) {
		models_[i]->Draw(partsWorldTransforms_[i], viewProjecion);
	}

}