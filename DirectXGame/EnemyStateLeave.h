#pragma once
#include "BaseEnemyState.h"
class EnemyStateLeave : public BaseEnemyState {
public:
	// コンストラクタ
	EnemyStateLeave(Enemy* enemy);
	~EnemyStateLeave();
	void Update() override;
	void showState() override;
};
