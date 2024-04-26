#include"EnemyStateApproach.h"
#include"Enemy.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) 
: BaseEnemyState("State Approach", enemy) {

}

EnemyStateApproach::~EnemyStateApproach() {

}

void EnemyStateApproach::Update() { 
	enemy_->Move({0.0f, 0.0f, -0.1f});
	//発射タイマーカウントダウン
	enemy_->FireTimerDecrement();
	//指定時間に達した
	if (enemy_->GetFireTimer() <= 0) {
	//弾を発射
		enemy_->Fire();
		//発射タイマー初期化
		enemy_->SetFireTimer(Enemy::kFireInterval);
	}
	showState();
}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}