#include "CollisionManager.h"

CollisionManager::CollisionManager(){};

CollisionManager::~CollisionManager(){};

void CollisionManager::EntryList(Collider*collider) {
	// コライダーをリストに登録
	colliders_.push_back(collider);
}

void CollisionManager::ClearList() {
	// コライダーをリストに登録
	colliders_.clear();
}

void CollisionManager::ChecAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレーターBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	
	Vector3 posA, posB;
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() ^ colliderB->GetCollisionMask()) == 0xFFFFFFFF || (colliderB->GetCollisionAttribute() ^ colliderA->GetCollisionMask()) == 0xFFFFFFFF) {
		return;
	}

	posA = colliderA->GetWorldPos();
	posB = colliderB->GetWorldPos();
	float distaince = powf((posA.x - posB.x), 2) + powf((posA.y - posB.y), 2) + powf((posA.z - posB.z), 2);

	// 球と球の交差判定
	if (distaince <= 5 + 5) {
		// 自キャラ衝突時コールバックを呼び出す
		colliderA->OnColligion();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnColligion();
	}
}
