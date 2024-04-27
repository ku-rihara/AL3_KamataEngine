#include"EnemyStateApproach.h"
#include"Enemy.h"


EnemyStateApproach::EnemyStateApproach(Enemy* enemy) 
: BaseEnemyState("State Approach", enemy) {

}

EnemyStateApproach::~EnemyStateApproach() {

}

void EnemyStateApproach::Update() { 
	enemy_->Move({0.0f, 0.0f, -0.1f});
	showState();
	// タイマー更新
	for (TimedCall* timedCall : enemy_->GetTimedCalls()) {
			timedCall->Update();
	}

}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}