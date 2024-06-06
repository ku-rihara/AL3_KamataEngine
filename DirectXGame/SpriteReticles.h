#pragma once
#include"WorldTransform.h"
#include "ViewProjection.h"
#include "Sprite.h"

class Player;
class Enemy;
class GameScene;


class SpriteReticles {
private:
	// 包含
	Player* player_;
	GameScene* gameScene_;
	Enemy* targetEnemy_;

	Sprite* sprite2DReticle_ = nullptr;
	/*Vector2 Reticle2DPos_;*/
	WorldTransform worldTransform3DReticle_;
	uint32_t textureReticle;

public:
	SpriteReticles();
	~SpriteReticles();
	void Init(Enemy* targetEnemy);
	void Updata();
	void Draw();
	void OnEnemyDeath();
	
	Vector2 Get2DReticlePos() const { return sprite2DReticle_->GetPosition(); }

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};

