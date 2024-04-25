#pragma once
#include"BaseEnemyState.h"

class EnemyStateApproach : public BaseEnemyState {
public:
	//コンストラクタ
	EnemyStateApproach(Enemy* enemy);
	~EnemyStateApproach();
	void Update()override;
	void showState() override;
};
