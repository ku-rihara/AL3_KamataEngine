#include "SpriteReticles.h"
#include "Easing.h"
#include "Geometry/fMatrix4x4.h"
#include "Reticle2D.h"
#include "TextureManager.h"
#include <algorithm>
// class
#include "Enemy.h"
#include "GameScene.h"
#include "Player.h"
#include "WinApp.h"

SpriteReticles::SpriteReticles() {}

SpriteReticles::~SpriteReticles() {}

void SpriteReticles::Init(Enemy* targetEnemy) {
	// レティクル用テクスチャ取得
	targetEnemy_ = targetEnemy;
	textureReticle = TextureManager::Load("anchorPoint.png");
	worldTransform3DReticle_.Initialize();
	
		sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(targetEnemy_->GetScreenPos().x, targetEnemy_->GetScreenPos().y), Vector4(1, 0, 0, 1), Vector2(0.5f, 0.5f));
	
}

void SpriteReticles::Updata() {
	
		sprite2DReticle_->SetPosition(targetEnemy_->GetScreenPos());
	
}

void SpriteReticles::Draw() {
	
		sprite2DReticle_->Draw();
	
}


