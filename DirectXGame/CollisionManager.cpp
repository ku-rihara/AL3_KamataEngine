#include "CollisionManager.h"

void CollisionManager::AddCollider(Colider* collider) {
	// コライダーをリストに登録
	colliders_.push_back(collider);
}

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
	if (distance <= 1 +3) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	
	}
}

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Colider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Colider* colliderA = *itrA;//ダブルポインタから中身のポインタを取り出す処理

		// イテレーターBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Colider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Colider* colliderB = *itrB;//ダブルポインタから中身のポインタを取り出す処理

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}
