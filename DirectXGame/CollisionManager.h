#pragma once
#include "Collider.h"
#include <list>
class CollisionManager {
private:
	std::list<Collider*> colliders_;

private:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

public:
	CollisionManager();
	~CollisionManager();

	void EntryList(Collider* collider);
	void ClearList();
	void ChecAllCollisions();
};
