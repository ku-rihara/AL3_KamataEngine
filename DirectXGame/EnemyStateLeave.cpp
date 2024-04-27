#include "EnemyStateLeave.h"
#include "Enemy.h"


EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {}

EnemyStateLeave::~EnemyStateLeave() {}

void EnemyStateLeave::Update() {
	showState();
	enemy_->Move({-0.1f, 0.1f, 0});
}
void EnemyStateLeave::showState() { BaseEnemyState::showState(); }
