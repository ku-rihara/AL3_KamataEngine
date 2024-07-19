#pragma once
#include<memory>
#include<list>
#include"Vector3.h"
#include"ViewProjection.h"
#include"Sprite.h"

class Enemy;
class LockOn {
private:
	//ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;
	//ロックオン対象
	const Enemy* target_ = nullptr;
	bool isRockOn_=false;

	float kDegreeToRadian = 3.14f / 4.0f;
	//最小距離
	float minDistance_ = 10.0f;
	//最大距離
	float maxDistance_ = 30.0f;
	//角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian;

public:
	void Init();
	void Update(const std::list<std::unique_ptr<Enemy>>&enemies,const ViewProjection&viewProjection);
	void Draw();

	void SetEnemy( Enemy* player) { target_ = player; }
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);
};
