#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"
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
	if (enemy_->GetWorldTransform().z <= 0) {
		enemy_->GetTimedCalls().clear();
		isShowState_ = false;
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}