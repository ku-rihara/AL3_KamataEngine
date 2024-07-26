#include "Hummer.h"
#include"Easing.h"
#include"Pi.h"
#include"CollisionTypeIdDef.h"
//class
#include"Enemy.h"
Hummer::Hummer() {}

void Hummer::SetParent(const WorldTransform& worldTransform) { 	
	worldTransform_.parent_ = &worldTransform; 
}

void Hummer::Init(Model* models) { 
	
	worldTransform_.Initialize();
	model_=models;
	Colider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
	// コライダー初期化
	Colider::Init();
}

void Hummer::Update() { worldTransform_.UpdateMatrix(); }

void Hummer::Attack(float easeT) { 
	worldTransform_.rotation_.x = Lerp(-pi / 3, pi / 2, easeT); 
}

void Hummer::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection); 
}


Vector3 Hummer::GetBaseCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 9.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Hummer::OnCollision([[maybe_unused]] Colider* other){
	//衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);
		enemy;
	}
}

