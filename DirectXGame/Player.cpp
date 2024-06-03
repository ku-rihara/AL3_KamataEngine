#include "Player.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "cassert"
//class
#include"Enemy.h"

Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Init(Model* model, uint32_t textureHandle) {
	input_ = Input::GetInstance();
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_.z = 30;
	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("anchorPoint.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(640, 320), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f));
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void Player::Update(const ViewProjection& viewProjection) {

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	// 自機から３Dレティクルへのオフセット（Z）
	Vector3 offset = {0, 0, kDistancePlayerTo3DReticle};
	// 自機のワールド行列の回転を反映
	offset = Multiply(offset, worldTransform_.matWorld_);
	// 自機の長さを整える
	offset = Normnalize(offset) * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = GetWorldPos() + offset;
	worldTransform3DReticle_.UpdateMatrix();

	if (isRockOn_) {
		sprite2DReticle_->SetColor(Vector4{1, 0, 0, 0});
	} else {
		sprite2DReticle_->SetColor(Vector4{1, 1,1, 1});
	}

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

	worldTransform_.UpdateMatrix(); // 行列更新

	Vector3 positionReticle = GetWorld3DRecticlPos();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewPrijectionViewPort = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド➩スクリーン変換（3Dから2D）
	positionReticle = Transform(positionReticle, matViewPrijectionViewPort);
	// スプライトのレティクルに座標変換
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	ImGui::Begin("player");
	ImGui::Text("PosX%f", worldTransform_.translation_.x);
	ImGui::Text("PosY%f", worldTransform_.translation_.y);
	ImGui::Text("PosZ%f", worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() {
	// 3Dレティクル
	sprite2DReticle_->Draw();
}

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

			 velocity = worldTransform3DReticle_.translation_ - GetWorldPos();
			 velocity = worldTransform3DReticle_.translation_ - GetWorldPos();
			velocity = Normnalize(velocity) * kBulletSpeed;
		
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		    newBullet->SetEnemy(enemy_);
		newBullet->Init(model_, GetWorldPos(), velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
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

Vector3 Player::GetWorld3DRecticlPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

// setter
// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }