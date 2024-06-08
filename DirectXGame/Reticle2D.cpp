#include "Reticle2D.h"
#include "Easing.h"
#include "Geometry/fMatrix4x4.h"
#include "TextureManager.h"
#include <algorithm>
// class
#include "GameScene.h"
#include "Player.h"
#include "WinApp.h"

Reticle2D::Reticle2D() {}

Reticle2D::~Reticle2D() {

	for (SpriteReticles* reticle2D : sprite2DReticles_) {
		delete reticle2D;
	}
}

void Reticle2D::Init() {
	// レティクル用テクスチャ取得
	textureReticle = TextureManager::Load("anchorPoint.png");
	worldTransform3DReticle_.Initialize();
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(640, 320), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f));
}

void Reticle2D::Updata(const ViewProjection& viewProjection) {
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	// 自機から３Dレティクルへのオフセット（Z）
	Vector3 offset = {0, 0, kDistancePlayerTo3DReticle};
	// 自機のワールド行列の回転を反映
	offset = Multiply(offset, player_->GetMatWorld());
	// 自機の長さを整える
	offset = Normnalize(offset) * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = player_->GetWorldPos() + offset;
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = GetWorld3DRecticlPos();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewPrijectionViewPort = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド➩スクリーン変換（3Dから2D）
	positionReticle = Transform(positionReticle, matViewPrijectionViewPort);
	// スプライトのレティクルに座標変換
	Reticle2DPos_ = (Vector2(positionReticle.x, positionReticle.y));
	sprite2DReticle_->SetPosition(Reticle2DPos_);

	Enemy* targetEnemy = nullptr;

	for (Enemy* enemy : gameScene_->GetEnemys()) {

		if (enemy->GetIsTarget() && !enemy->GetIsRocked()) { //	ターゲットしてる
			targetEnemy = enemy;
			SpriteReticles* newSpriteReticle = new SpriteReticles();
			newSpriteReticle->SetPlayer(player_);
			newSpriteReticle->SetGameScene(gameScene_);
			newSpriteReticle->Init(targetEnemy);
			sprite2DReticles_.push_back(newSpriteReticle);
			enemy->SetIsRocked(true);
		}
	}
	// 複数レティクルの更新
	for (SpriteReticles* reticleSprite : sprite2DReticles_) {
		reticleSprite->Updata();
	}

	//敵が死んだらレティクルを削除
	for (auto it = sprite2DReticles_.begin(); it != sprite2DReticles_.end();) {
		SpriteReticles* reticleSprite = *it;
		bool shouldDelete = false;
		for (Enemy* enemy : gameScene_->GetEnemys()) {
			if (enemy->GetIsDeath()) {
				shouldDelete = true;
				break;
			}
		}
		if (shouldDelete) {
			delete reticleSprite;
			it = sprite2DReticles_.erase(it);
			break;
		} else {
			reticleSprite->Updata();
			++it;
		}
	}
}

void Reticle2D::Draw() {

	for (SpriteReticles* reticleSprite : sprite2DReticles_) {

		reticleSprite->Draw();
	}
	sprite2DReticle_->Draw();
}

Vector3 Reticle2D::GetWorld3DRecticlPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}
