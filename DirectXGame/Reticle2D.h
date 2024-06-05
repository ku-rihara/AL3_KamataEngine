#pragma once
#include "Sprite.h"
#include"WorldTransform.h"
#include "ViewProjection.h"
// std
#include <list>
class Player;
class GameScene;
class Reticle2D {
private:
	//包含
	Player* player_;
	GameScene* gameScene_;
	Sprite* sprite2DReticle_ = nullptr;
	Vector2 Reticle2DPos_;
	Vector2 rockPos_;
	float reticleMoveTime_;
	WorldTransform worldTransform3DReticle_;

public:
	Reticle2D();
	~Reticle2D();
	void Init();
	void Updata(const ViewProjection& viewProjection);
	void Draw();

	Vector3 GetWorld3DRecticlPos();
	Vector2 Get2DReticlePos() const { return Reticle2DPos_; }

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};
