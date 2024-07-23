#pragma once
#include<list>
#include"Colider.h"
class CollisionManager {
private:
	//コライダー
	std::list<Colider*> colliders_;

public:
	//リセット
	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="コライダーA"></param>
	/// <param name="コライダーB"></param>
	void CheckCollisionPair(Colider* colliderA, Colider* colliderB);
};
