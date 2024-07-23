#include "CollisionManager.h"

void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Colider* colliderA, Colider* colliderB) {
	//コライダーの座標取得
	Vector3 posA = colliderA->GetBaseCenterPosition();
	Vector3 posB = colliderB->GetBaseCenterPosition();
	//座標の差分ベクトル
	Vector3 subtract = posB - posA;
	//座標AとBの距離を求める
	float distance = Length(subtract);
	//球同士の交差判定
	if (distance>posA+posB) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	
	}
}