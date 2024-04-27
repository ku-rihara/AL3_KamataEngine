#include"EnemyStateApproach.h"
#include"Enemy.h"


EnemyStateApproach::EnemyStateApproach(Enemy* enemy) 
: BaseEnemyState("State Approach", enemy) {

}

EnemyStateApproach::~EnemyStateApproach() {

}

void EnemyStateApproach::Update() { 
	enemy_->Move({0.0f, 0.0f, -0.1f});
	// タイマー更新
	for (TimedCall* timedCall : enemy_->GetTimedCalls()) {
		timedCall->Update();
	}
	if (enemy_->GetWorldTransform().z <= 0) {
		enemy_->GetTimedCalls().clear();
	}
	showState();
}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}