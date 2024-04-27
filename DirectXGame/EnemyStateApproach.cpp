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
}
void EnemyStateApproach::showState() { 
BaseEnemyState::showState();
}