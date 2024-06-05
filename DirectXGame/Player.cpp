#include "Player.h"
#include "CollisionConfig.h"
#include "Easing.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "cassert"
// class
#include "Enemy.h"
#include "Reticle2D.h"
#include "GameScene.h"

Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

		for (Sprite* reticle2D : sprite2DReticles_) {
		delete reticle2D;
	}
	
}

void Player::Init(Model* model, uint32_t textureHandle) {
	input_ = Input::GetInstance();
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_.z = 30;
	worldTransform_.Initialize();

		// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void Player::Update() {

	ImGui::Begin("player");
	ImGui::Text("PosX%f", worldTransform_.translation_.x);
	ImGui::Text("PosY%f", worldTransform_.translation_.y);
	ImGui::Text("PosZ%f", worldTransform_.translation_.z);
	ImGui::End();

	

	// ですフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクター旋回処理
	Rotate();
	// キャラクターの移動処理
	Move();
	// キャラクターの攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	};

	worldTransform_.UpdateMatrix(); // プレイヤーの行列更新

}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

// その他関数*********************************************************************************************************
void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Move() {
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_ += move;

	// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_W)) {
		Vector3 velocity;
		// 弾の速度
		const float kBulletSpeed = 1.0f;

		velocity = reticle2D_->GetWorld3DRecticlPos() - GetWorldPos();
		velocity = Normnalize(velocity) * kBulletSpeed;

		 // 現在ロックオンしている敵を取得
		/*Enemy* targetEnemy = nullptr;
		if (isRockOn_) {
			for (Enemy* enemy : gameScene_->GetEnemys()) {
				if (enemy->GetIsTarget()) {
					targetEnemy = enemy;
					break;
				}
			}
		}*/ 

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->SetGameScene(gameScene_);
		newBullet->Init(model_, GetWorldPos(), velocity, isRockOn_);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::ReticleInit() {

}

void Player::OnColligion() {
	// 何もしない
}

void Player::RockOn() { isRockOn_ = true; }

void Player::CanselRockOn() { isRockOn_ = false; }

// Getter-------------------------------------------------------------
Vector3 Player::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

// setter
// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }