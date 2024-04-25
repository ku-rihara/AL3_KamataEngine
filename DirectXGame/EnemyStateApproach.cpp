#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"
#include"Enemy.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) 
: BaseEnemyState("State Approach", enemy) {

}

EnemyStateApproach::~EnemyStateApproach() {

}

void EnemyStateApproach::Update() { 
	enemy_->Move({0.0f, 0.0f, -0.3f});
	showState();
	if (enemy_->GetWorldTransform().z <= 0) {
		isShowState_ = true;
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
	
}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}